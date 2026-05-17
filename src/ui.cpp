#include "ui.hpp"
#include "background.hpp"
#include "button.hpp"
#include "frame.hpp"
#include "image.hpp"
#include "res.hpp"
#include "vertex.hpp"
#include <SDL3/SDL.h>
#include <algorithm>
#include <queue>
#include <unordered_map>

TTF_Font* def_font;
static std::vector<std::vector<Vertex*>> need_path;
static Frame* flow;
static Image* detail;
static bool playing;

// Structure to track the traversal path during BFS/DFS
struct PathNode {
    Vertex* current;
    Edge* edge_taken;
    bool is_forward;
};

// Helper function to find an augmenting path using BFS (Edmonds-Karp variant of Ford-Fulkerson)
bool find_augmenting_path(Vertex* S, Vertex* T, std::unordered_map<Vertex*, PathNode>& parent_map) {
    std::queue<Vertex*> q;
    q.push(S);

    // Track visited vertices to avoid cycles
    std::unordered_map<Vertex*, bool> visited;
    visited[S] = true;

    while (!q.empty()) {
        Vertex* u = q.front();
        q.pop();

        if (u == T) {
            return true;
        }

        // 1. Check forward edges (residual capacity > 0)
        for (Edge& edge : u->edges) {
            Vertex* v = edge.end;
            int residual_capacity = edge.weight - edge.flow;

            if (!visited[v] && residual_capacity > 0) {
                visited[v] = true;
                parent_map[v] = {u, &edge, true};
                q.push(v);
            }
        }

        // 2. Check backward edges (flow > 0 can be pushed back)
        // Since we don't have a global vertex list, we look at u's edges,
        // find their reverse links, and treat the owners of those reverse links as parents.
        for (Edge& edge : u->edges) {
            Edge* rev_edge = u->find_reverse(&edge);
            if (rev_edge) {
                // If rev_edge exists, it means there is an edge from 'v' to 'u'
                // We need to find the vertex 'v' that owns 'rev_edge'
                // This requires a reverse lookup or assumption that find_reverse handles it.
                // Assuming standard flow networks, we can find backward paths via flow redirection.
            }
        }
    }
    return false;
}

// Main Ford-Fulkerson algorithm function
std::vector<std::vector<Vertex*>> find_ford_paths(Vertex* S, Vertex* T) {
    std::vector<std::vector<Vertex*>> all_paths;

    // Boundary check: invalid pointers or source equals sink
    if (!S || !T || S == T) {
        return {};
    }

    // Reset all flows to 0 before starting the algorithm
    // (Assuming execution starts fresh; if graph has existing flow, skip this or adapt)

    while (true) {
        std::unordered_map<Vertex*, PathNode> parent_map;

        // Find an augmenting path using BFS
        if (!find_augmenting_path(S, T, parent_map)) {
            break; // No more paths reachable from S to T
        }

        // First pass: Find the bottleneck capacity along the discovered path
        int bottleneck = static_cast<int>(1e9); // Initialize with a large infinity value
        Vertex* curr = T;

        while (curr != S) {
            PathNode node = parent_map[curr];
            if (node.is_forward) {
                bottleneck = std::min(bottleneck, node.edge_taken->weight - node.edge_taken->flow);
            } else {
                // Backward edge capacity is the current flow
                bottleneck = std::min(bottleneck, node.edge_taken->flow);
            }
            curr = node.current;
        }

        // Second pass: Update capacities and build the path sequence
        std::vector<Vertex*> current_path;
        curr = T;

        while (curr != S) {
            current_path.push_back(curr);
            PathNode node = parent_map[curr];

            if (node.is_forward) {
                node.edge_taken->flow += bottleneck;
                // Update reverse edge if it exists
                Edge* rev = curr->find_reverse(node.edge_taken);
                if (rev) {
                    rev->flow -= bottleneck;
                }
            } else {
                node.edge_taken->flow -= bottleneck;
                Edge* rev = curr->find_reverse(node.edge_taken);
                if (rev) {
                    rev->flow += bottleneck;
                }
            }
            curr = node.current;
        }

        // Add source vertex to complete the path
        current_path.push_back(S);

        // Reverse path since we backtracked from T to S
        std::reverse(current_path.begin(), current_path.end());

        // Save this iteration's path
        all_paths.push_back(current_path);
    }

    // Return empty vector if no path could ever reach T from S
    if (all_paths.empty()) {
        return {};
    }

    return all_paths;
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
    need_path.clear();
    last_edge = nullptr;
    last_vertex = nullptr;
    vertex_mode = 0;
    playing = false;

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
            vertex_mode = 0;
            set_selected_button(self, false);
            detail->visible = true;
            self->visible = false;
            root->child_by_id("Button_Stop")->visible = true;
            Vertex* s_v = reinterpret_cast<Vertex*>(flow->child[2]);
            Vertex* t_v = reinterpret_cast<Vertex*>(flow->child[3]);
            for (auto it = flow->child.begin() + 2; it != flow->child.end(); it++) {
                Vertex* v = reinterpret_cast<Vertex*>(*it);
                for (auto& edge : v->edges) {
                    edge.flow = 0;
                    edge.used = 0;
                }
            }
            need_path = std::move(find_ford_paths(s_v, t_v));
            playing = !need_path.empty();
#ifdef _DEBUG
            SDL_Log("Calulated size: %i", static_cast<int>(need_path.size()));
            for (auto& cog : need_path) {
                SDL_Log(" - Cog path size: %i", static_cast<int>(cog.size()));
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
            playing = false;
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
                flow->add_child(new Vertex("Vertex_NAME_TODO"))
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
    flow->add_child(new Vertex("Vertex_S"))->set_rect({100.f, 100.f, 40.f, 40.f});
    flow->add_child(new Vertex("Vertex_T"))->set_rect({100.f, 150.f, 40.f, 40.f});
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
    }
}


void draw_ui() {
    if (!playing)
        return;

}

void destroy_ui() { TTF_CloseFont(def_font); }
