#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <chrono>
#include <algorithm>
#include <random>

using namespace std;

// --- 1. FONCTION DE CHRONOMeTRAGE ---
template <typename Function>
double measure_time(Function func) {
    auto start = chrono::high_resolution_clock::now();
    func();
    auto end = chrono::high_resolution_clock::now();
    // Retourne le temps en millisecondes (ms)
    return chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;
}

// --- 2. FONCTION POUR LIRE LES DONNeES ---
vector<string> read_data_from_file(const string& filename) {
    vector<string> data;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Erreur: Impossible d'ouvrir le fichier " << filename << endl;
        // Quitte l'application
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

// --- 3. FONCTIONS D'OPeRATION SPeCIFIQUES ---

// --- Operations pour std::set et std::unordered_set ---  
template <typename SetType>
void run_set_tests(const string& name, const vector<string>& data) {
    SetType container;
    
    // Insertion
    double insert_time = measure_time([&]() {
        for (const auto& str : data) {
            container.insert(str);
        }
    });

    cout << "\n## " << name << endl;
    cout << "Insertion de " << data.size() << " elements: **" << insert_time << " ms**" << endl;

    // Melange pour un test non biaise
    vector<string> data_to_process = data; 
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(data_to_process.begin(), data_to_process.end(), default_random_engine(seed));
    
    // Recherche
    long long found_count = 0;
    double search_time = measure_time([&]() {
        for (const auto& str : data_to_process) {
            if (container.count(str)) {
                found_count++;
            }
        }
    });

    cout << "Recherche de " << data.size() << " elements (" << found_count << " trouves): **" << search_time << " ms**" << endl;

    // Suppression
    long long erase_count = 0;
    double erase_time = measure_time([&]() {
        for (const auto& str : data_to_process) {
            erase_count += container.erase(str);
        }
    });
    
    cout << "Suppression de " << data.size() << " elements (" << erase_count << " supprimes): **" << erase_time << " ms**" << endl;
    cout << "Taille finale du conteneur: " << container.size() << endl;
}

// --- Operations pour std::map ---
// Cette fonction est specialisee car la map insère des paires
void run_map_tests(const string& name, const vector<string>& data) {
    map<string, string> container;
    
    // Insertion
    double insert_time = measure_time([&]() {
        for (const auto& str : data) {
            // Insertion de la paire {cle, valeur}
            container.insert({str, str});
        }
    });

    cout << "\n## " << name << endl;
    cout << "Insertion de " << data.size() << " elements: **" << insert_time << " ms**" << endl;

    // Melange pour un test non biaise
    vector<string> data_to_process = data; 
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(data_to_process.begin(), data_to_process.end(), default_random_engine(seed));
    
    // Recherche
    long long found_count = 0;
    double search_time = measure_time([&]() {
        for (const auto& str : data_to_process) {
            if (container.count(str)) {
                found_count++;
            }
        }
    });

    cout << "Recherche de " << data.size() << " elements (" << found_count << " trouves): **" << search_time << " ms**" << endl;

    // Suppression
    long long erase_count = 0;
    double erase_time = measure_time([&]() {
        for (const auto& str : data_to_process) {
            erase_count += container.erase(str);
        }
    });
    
    cout << "Suppression de " << data.size() << " elements (" << erase_count << " supprimes): **" << erase_time << " ms**" << endl;
    cout << "Taille finale du conteneur: " << container.size() << endl;
}


// --- 4. FONCTION PRINCIPALE ---
int main() {
    const string FILENAME = "data.txt";

    // 1. Lire les donnees
    vector<string> strings_data = read_data_from_file(FILENAME);
    if (strings_data.empty()) {
        cerr << "Le fichier " << FILENAME << " est vide. Utilisez le generateur pour le creer." << endl;
        return 1;
    }
    cout << "--- Demarrage des tests avec " << strings_data.size() << " elements ---" << endl;

    // 2. Executer les tests pour chaque conteneur
    
    // std::set (utilise la fonction run_set_tests)
    run_set_tests<set<string>>("std::set", strings_data); 

    // std::unordered_set (utilise la fonction run_set_tests)
    run_set_tests<unordered_set<string>>("std::unordered_set", strings_data);

    // std::map (utilise la fonction run_map_tests specialisee)
    run_map_tests("std::map", strings_data); 
    
    cout << "\n--- Fin des tests ---" << endl;

    return 0;
}