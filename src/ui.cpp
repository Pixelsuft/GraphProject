#include "ui.hpp"
#include "background.hpp"
#include "button.hpp"
#include "clock.hpp"
#include "frame.hpp"
#include "image.hpp"
#include "res.hpp"
#include "text.hpp"
#include "vertex.hpp"
#include <SDL3/SDL.h>
#include <algorithm>
#include <queue>
#include <unordered_map>

TTF_Font* def_font;
void* flow_text;
static std::vector<std::pair<std::vector<Vertex*>, int>> need_path;
static Frame* flow;
static Image* detail;
static float timer;
static int prev_i;
static int total;
static bool fast;

// Structure to track the traversal path during BFS
struct PathNode {
    Vertex* current;
    Edge* edge_taken;
    bool is_forward;
};

bool find_augmenting_path(Vertex* S, Vertex* T, std::unordered_map<Vertex*, PathNode>& parent_map) {
    std::queue<Vertex*> q;
    q.push(S);

    std::unordered_map<Vertex*, bool> visited;
    visited[S] = true;

    while (!q.empty()) {
        Vertex* u = q.front();
        q.pop();

        if (u == T)
            return true;

        // Forward edges (residual capacity > 0)
        for (Edge& e : u->edges) {
            Vertex* v = e.end;
            int residual = e.weight - e.flow;
            if (!visited[v] && residual > 0) {
                visited[v] = true;
                parent_map[v] = {u, &e, true};
                q.push(v);
            }
        }

        // Backward edges (reverse edge has flow > 0)
        for (Edge& e : u->edges) {
            Vertex* v = e.end;
            Edge* rev = v->find_reverse(&e); // Edge v -> u
            if (rev && rev->flow > 0 && !visited[v]) {
                visited[v] = true;
                parent_map[v] = {u, rev, false}; // Taking the reverse edge backwards
                q.push(v);
            }
        }
    }
    return false;
}

std::vector<std::pair<std::vector<Vertex*>, int>> find_ford_paths(Vertex* S, Vertex* T) {
    std::vector<std::pair<std::vector<Vertex*>, int>> result;

    while (true) {
        std::unordered_map<Vertex*, PathNode> parent_map;
        if (!find_augmenting_path(S, T, parent_map))
            break;

        // Find bottleneck
        int bottleneck = INT_MAX;
        Vertex* curr = T;
        while (curr != S) {
            PathNode& pn = parent_map[curr];
            if (pn.is_forward)
                bottleneck = std::min(bottleneck, pn.edge_taken->weight - pn.edge_taken->flow);
            else
                bottleneck = std::min(
                    bottleneck, pn.edge_taken->flow); // backward capacity = flow of reverse edge
            curr = pn.current;
        }

        // Update flows and collect path
        std::vector<Vertex*> path;
        curr = T;
        while (curr != S) {
            path.push_back(curr);
            PathNode& pn = parent_map[curr];

            if (pn.is_forward) {
                pn.edge_taken->flow += bottleneck;
                Edge* rev = curr->find_reverse(pn.edge_taken);
                if (rev)
                    rev->flow -= bottleneck;
            } else {
                pn.edge_taken->flow -= bottleneck; // The reverse edge we took
                Edge* rev = curr->find_reverse(pn.edge_taken);
                if (rev)
                    rev->flow += bottleneck; // The original forward edge
            }
            curr = pn.current;
        }
        path.push_back(S);
        std::reverse(path.begin(), path.end());

        result.emplace_back(std::move(path), bottleneck);
    }
    return result;
}

static void set_selected_button(Container* btn, bool enabled = true) {
    // Hacky way to allow toggle
    if (reinterpret_cast<Button*>(btn)->bg_color.g != 0.f)
        vertex_mode = 0;
    for (const char* id :
         {"Button_Add", "Button_Join", "Button_Trash", "Button_Start", "Button_Stop"}) {
        Button* b = reinterpret_cast<Button*>(root->child_by_id(id));
        b->bg_color.g = (btn != b || vertex_mode == 0) ? 0.f : 0.2f;
        if (b->id != "Button_Start" && b->id != "Button_Stop") {
            b->enabled = enabled;
            b->border_color = enabled ? Color(0.f, 1.f, 0.f) : Color(1.f, 0.f, 0.f);
        }
        b->refresh_color();
    }
}

void construct_ui() {
    def_font = res->load_font("VCR_OSD_MONO.ttf", 24.f);
    flow_text = text->create_text(def_font);
    text->set_color(flow_text, Color(0.f, 1.f, 0.f));
    last_edge = nullptr;
    last_vertex = nullptr;
    vertex_mode = 0;
    fast = false;

    // Workflow
    flow = new Frame("Frame_Flow");
    root->add_child(flow)->set_resize_handler(
        [&](Container* self, Container* parent) { self->rect = parent->rect; });

    // Add button
    root->add_child(new Button("Button_Add"))
        ->set_child((new Image("Image_Add"))->set_texture(res->load_texture("add.png")))
        ->set_click_handler([&](Button* self, Container*) {
            vertex_mode = 1;
            set_selected_button(self);
        })
        ->set_rect({10.f, 10.f, 64.f, 64.f});
    // Join button
    root->add_child(new Button("Button_Join"))
        ->set_child((new Image("Image_Join"))->set_texture(res->load_texture("join.png")))
        ->set_click_handler([&](Button* self, Container*) {
            vertex_mode = 2;
            set_selected_button(self);
        })
        ->set_rect({84.f, 10.f, 64.f, 64.f});
    // Trash button
    root->add_child(new Button("Button_Trash"))
        ->set_child((new Image("Image_Trash"))->set_texture(res->load_texture("trash.png")))
        ->set_click_handler([&](Button* self, Container*) {
            vertex_mode = 3;
            set_selected_button(self);
        })
        ->set_rect({158.f, 10.f, 64.f, 64.f});
    // Start button
    root->add_child(new Button("Button_Start"))
        ->set_child((new Image("Image_Start"))->set_texture(res->load_texture("start.png")))
        ->set_click_handler([&](Button* self, Container*) {
            last_edge = nullptr;
            Vertex* s_v = reinterpret_cast<Vertex*>(flow->child[2]);
            Vertex* t_v = reinterpret_cast<Vertex*>(flow->child[3]);
            need_path = find_ford_paths(s_v, t_v);
            if (need_path.empty())
                return;
            vertex_mode = 0;
            set_selected_button(self, false);
            self->visible = false;
            root->child_by_id("Button_Stop")->visible = true;
            std::reverse(need_path.begin(), need_path.end());
            timer = 0.f;
            total = 0;
            prev_i = -1;
#ifdef _DEBUG
            SDL_Log("Calulated size: %i", static_cast<int>(need_path.size()));
            for (auto& cog : need_path) {
                SDL_Log(" - Cog path size: %i (repeat x%i)", static_cast<int>(cog.first.size()),
                        cog.second);
            }
#endif
        })
        ->set_rect({232.f, 10.f, 64.f, 64.f});
    // Stop button
    root->add_child(new Button("Button_Stop"))
        ->set_child((new Image("Image_Stop"))->set_texture(res->load_texture("stop.png")))
        ->set_click_handler([&](Button* self, Container*) {
            last_edge = nullptr;
            vertex_mode = 0;
            set_selected_button(self, true);
            detail->visible = false;
            self->visible = false;
            root->child_by_id("Button_Start")->visible = true;
            need_path.clear();
            for (auto it = flow->child.begin() + 2; it != flow->child.end(); it++) {
                Vertex* v = reinterpret_cast<Vertex*>(*it);
                for (auto& edge : v->edges) {
                    edge.flow = 0;
                    edge.used = 0;
                    edge.update_text();
                }
            }
        })
        ->set_rect({232.f, 10.f, 64.f, 64.f});
    root->child_by_id("Button_Stop")->visible = false;

    // Background
    flow->add_child(new Background("Background"))
        ->set_down_handler([&](Background*, Container*, Point pos) {
            if (vertex_mode == 1) {
                // push_back might reallocate memory
                last_vertex = nullptr;
                last_edge = nullptr;
                flow->add_child(new Vertex("Vertex_С"))
                    ->set_rect(Rect({pos.x - 20.f, pos.y - 20.f, 40.f, 40.f}));
            }
        })
        ->set_resize_handler(
            [&](Container* self, Container* parent) { self->rect = parent->rect; });
    // Detail for demo
    detail = new Image("Detail", true);
    detail->visible = false;
    flow->add_child(detail)
        ->set_texture(res->load_texture("cogwheel.png"))
        ->set_rect({0.f, 0.f, 32.f, 32.f});
    // S, T vertexes
    flow->add_child(new Vertex("Vertex_S"))->set_rect({100.f, 120.f, 40.f, 40.f});
    flow->add_child(new Vertex("Vertex_T"))->set_rect({300.f, 300.f, 40.f, 40.f});
}

void kbd_ui(char key) {
    if (key == '+' && last_edge) {
        last_edge->weight++;
        last_edge->update_text();
    } else if (key == '-' && last_edge) {
        if (last_edge->weight > 1) {
            last_edge->weight--;
            last_edge->update_text();
        } else {
            auto index = last_edge - last_vertex->edges.data();
            last_vertex->edges.erase(last_vertex->edges.begin() + index);
            last_edge = nullptr;
        }
    } else if (key == 'r' && need_path.empty()) {
        auto prev_mode = vertex_mode;
        vertex_mode = 3;
        while (flow->child.size() > 4)
            flow->child.back()->on_mouse_down(flow, Point(), 0, true);
        vertex_mode = prev_mode;
    } else if (key == 'f')
        fast = !fast;
}

void draw_ui() {
    text->set_text(flow_text, (std::string("Flow: ") + std::to_string(total)).c_str());
    text->draw(flow_text, Point(10.f, 84.f), false);
    if (need_path.empty()) {
        Point need_pos = reinterpret_cast<Vertex*>(flow->child[3])->get_center();
        detail->rect.x = need_pos.x - detail->rect.w / 2.f;
        detail->rect.y = need_pos.y - detail->rect.h / 2.f;
        return;
    }
    detail->visible = true;

    auto& [cur_track, bottleneck] = need_path.back();

    timer += gclock->dt * (fast ? 8.f : 1.f);
    int cur_index = static_cast<int>(timer);
    float perc = timer - static_cast<float>(cur_index);

    if (cur_index >= static_cast<int>(cur_track.size()) - 1) {
        bottleneck--;
        total++;
        if (bottleneck != 0) {
            timer = 0.f;
            prev_i = -1;
        } else {
            need_path.pop_back();
            timer = 0.f;
            prev_i = -1;
        }
        return;
    }

    if (prev_i != cur_index) {
        prev_i = cur_index;
        Edge* e = cur_track[cur_index]->find_connection(cur_track[cur_index + 1]);
        if (e) {
            e->used++;
            e->update_text();
        }
    }
    Point start = cur_track[cur_index]->get_center();
    Point route = cur_track[cur_index + 1]->get_center() - start;
    if (route.empty())
        return;
    Point need_pos = start + route * perc;
    detail->rect.x = need_pos.x - detail->rect.w / 2.f;
    detail->rect.y = need_pos.y - detail->rect.h / 2.f;
}

void destroy_ui() {
    text->destroy_text(flow_text);
    TTF_CloseFont(def_font);
}
