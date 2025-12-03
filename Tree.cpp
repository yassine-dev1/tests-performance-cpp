#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <chrono>
#include <algorithm>
#include <random>

using namespace std;

// --- UTILS ET CHRONOMeTRAGE (Inchange ) ----

template <typename Function>
double measure_time(Function func) {
    auto start = chrono::high_resolution_clock::now();
    func();
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;
}

vector<string> read_data_from_file(const string& filename) {
    vector<string> data;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Erreur: Impossible d'ouvrir le fichier " << filename << endl;
        exit(1); 
    }
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            data.push_back(line);
        }
    }
    file.close();
    return data;
}

// --- 2. IMPLeMENTATION DE L'ABR SIMPLE (Mise à Jour) ---

struct Node {
    string key;
    Node* left;
    Node* right;

    Node(const string& val) : key(val), left(nullptr), right(nullptr) {}
    // Destructeur (simplifie pour le test, le cleanup sera fait par delete_tree)
};

// Fonction utilitaire pour trouver le noeud avec la cle la plus petite dans un sous-arbre
Node* find_min_node(Node* node) {
    Node* current = node;
    while (current && current->left != nullptr) {
        current = current->left;
    }
    return current;
}

// Insertion dans l'ABR (Inchangee)
void insert_bst(Node*& root, const string& key) {
    if (root == nullptr) {
        root = new Node(key);
        return;
    }
    if (key < root->key) {
        insert_bst(root->left, key);
    } else if (key > root->key) {
        insert_bst(root->right, key);
    }
}

// Recherche dans l'ABR (Inchangee)
bool search_bst(Node* root, const string& key) {
    if (root == nullptr) return false;
    if (key == root->key) return true;
    return (key < root->key) ? search_bst(root->left, key) : search_bst(root->right, key);
}

// Suppression dans l'ABR
Node* delete_bst(Node* root, const string& key) {
    if (root == nullptr) {
        return root;
    }

    if (key < root->key) {
        // La cle est dans le sous-arbre gauche
        root->left = delete_bst(root->left, key);
    } else if (key > root->key) {
        // La cle est dans le sous-arbre droit
        root->right = delete_bst(root->right, key);
    } else {
        // Cle trouvee: On gère les trois cas de suppression
        Node* temp = root;

        // Cas 1: Le nœud n'a qu'un seul enfant ou aucun enfant
        if (root->left == nullptr) {
            root = root->right;
            delete temp; // Libère l'ancien nœud
            return root;
        } else if (root->right == nullptr) {
            root = root->left;
            delete temp;
            return root;
        }

        // Cas 3: Le nœud a deux enfants
        // Trouve le successeur immediat (le plus petit dans le sous-arbre droit)
        temp = find_min_node(root->right);

        // Copie le contenu du successeur dans le nœud courant
        root->key = temp->key;

        // Supprime le successeur
        root->right = delete_bst(root->right, temp->key);
    }
    return root;
}

// Fonction de nettoyage pour eviter les fuites de memoire après le test
void delete_tree(Node* root) {
    if (root) {
        delete_tree(root->left);
        delete_tree(root->right);
        delete root;
    }
}

// --- 3. FONCTION DE TEST POUR L'ABR SIMPLE (Mise à Jour) ---

void run_bst_simple_tests(const vector<string>& data) {
    Node* root = nullptr;
    
    // Insertion
    double insert_time = measure_time([&]() {
        for (const auto& str : data) {
            insert_bst(root, str);
        }
    });

    cout << "\n## 1. Arbre Binaire de Recherche " << endl;
    cout << "Insertion de " << data.size() << " elements: **" << insert_time << " ms**" << endl;

    vector<string> data_to_process = data; 
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(data_to_process.begin(), data_to_process.end(), default_random_engine(seed));
    
    // Recherche
    long long found_count = 0;
    double search_time = measure_time([&]() {
        for (const auto& str : data_to_process) {
            if (search_bst(root, str)) {
                found_count++;
            }
        }
    });

    cout << "Recherche de " << data.size() << " elements: **" << search_time << " ms**" << endl;
    
    // Suppression
    long long erase_count = 0;
    double erase_time = measure_time([&]() {
        for (const auto& str : data_to_process) {
            // Tente de supprimer la cle
            Node* old_root = root;
            root = delete_bst(root, str); 
            
            // Simple verification de succès (on suppose que si delete_bst est appele, il y a suppression)
            // Une implementation plus stricte verifierait si la cle existe avant/après.
            if (root != old_root || (root == nullptr && old_root != nullptr) || (root != nullptr && old_root != root)) {
                erase_count++; 
            }
        }
    });

    cout << "Suppression de " << data_to_process.size() << " elements: **" << erase_time << " ms**" << endl;
    
    // Nettoyage de la memoire finale
    delete_tree(root); 
}


// --- 4. FONCTION DE TEST POUR L'ARBRE BICOLORE (RBT via std::set) ---
// Cette fonction reste inchangee par rapport au code precedent, car std::set gère toutes les operations O(log N).

void run_rbt_tests(const vector<string>& data) {
    set<string> container;
    
    // Insertion
    // ... (Logique inchangee : insertion O(log N)) ...
    double insert_time = measure_time([&]() {
        for (const auto& str : data) {
            container.insert(str);
        }
    });
    cout << "\n## 2. Arbre Bicolore " << endl;
    cout << "Insertion de " << data.size() << " elements: **" << insert_time << " ms**" << endl;

    vector<string> data_to_process = data; 
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(data_to_process.begin(), data_to_process.end(), default_random_engine(seed));
    
    // Recherche
    // ... (Logique inchangee : recherche O(log N)) ...
    long long found_count = 0;
    double search_time = measure_time([&]() {
        for (const auto& str : data_to_process) {
            if (container.count(str)) {
                found_count++;
            }
        }
    });
    cout << "Recherche  de " << data.size() << " elements: **" << search_time << " ms**" << endl;

    // Suppression O(log N)
    long long erase_count = 0;
    double erase_time = measure_time([&]() {
        for (const auto& str : data_to_process) {
            erase_count += container.erase(str);
        }
    });
    
    cout << "Suppression de " << data_to_process.size() << " elements: **" << erase_time << " ms**" << endl;
    cout << "Taille finale du conteneur: " << container.size() << endl;
}


// --- 5. FONCTION PRINCIPALE (Inchangee) ---

int main() {
    const string FILENAME = "data.txt";
    vector<string> strings_data = read_data_from_file(FILENAME);
    
    if (strings_data.empty()) {
        return 1;
    }
    cout << "--- Demarrage des tests ABR vs RBT avec " << strings_data.size() << " elements ---" << endl;
    
    run_bst_simple_tests(strings_data); 
    run_rbt_tests(strings_data); 
    
    cout << "\n--- Fin des tests ---" << endl;

    return 0;
}