#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <unordered_set>

using namespace std;

// Fonction pour générer une chaîne de caractères aléatoire de longueur fixe
string generate_random_string(size_t length) {
    const string CHARACTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    // Utilise un générateur de nombres aléatoires Mersenne Twister
    static mt19937 generator(chrono::system_clock::now().time_since_epoch().count());
    // Distribution uniforme pour choisir les caractères
    uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    string random_string;
    for (size_t i = 0; i < length; ++i) {
        random_string += CHARACTERS[distribution(generator)];
    }
    return random_string;
}

int main() {
    const string FILENAME = "data.txt";
    const int NUM_STRINGS = 1000000; // Un million de chaînes
    const size_t STRING_LENGTH = 15; // Chaque chaîne aura 15 caractères

    cout << "Création du fichier " << FILENAME << " avec " << NUM_STRINGS << " chaînes..." << endl;

    ofstream file(FILENAME);

    if (!file.is_open()) {
        cerr << "Erreur: Impossible d'ouvrir le fichier pour l'écriture." << endl;
        return 1;
    }

    // Un set pour garantir l'unicité des chaînes
    unordered_set<string> unique_strings;
    
    // Boucle pour générer et écrire un million de chaînes uniques
    while (unique_strings.size() < NUM_STRINGS) {
        string random_str = generate_random_string(STRING_LENGTH);
        // Tente d'insérer. Si l'insertion est réussie (la chaîne est nouvelle), l'insérer dans le fichier.
        if (unique_strings.insert(random_str).second) {
            file << random_str << "\n";
        }
    }

    file.close();
    cout << "Génération terminée. " << NUM_STRINGS << " chaînes uniques écrites dans " << FILENAME << "." << endl;

    return 0;
}