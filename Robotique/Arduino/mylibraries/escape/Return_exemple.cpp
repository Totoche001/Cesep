#include <iostream>
#include <string>

// Fonction pour calculer le prix total des fruits
double calculerPrixTotal(int nombrePrunes, int nombrePoires) {
    const double prixPrune = 0.5;  // Prix d'une prune en euros
    const double prixPoire = 0.7;  // Prix d'une poire en euros
    
    double prixTotal = (nombrePrunes * prixPrune) + (nombrePoires * prixPoire);
    
    return prixTotal;  // Retourne le prix total calculé
}

int main() {
    int prunes = 5;
    int poires = 3;
    
    double total = calculerPrixTotal(prunes, poires);
    
    std::cout << "Prix total pour " << prunes << " prunes et " << poires << " poires : " << total << " euros" << std::endl;
    
    return 0;  // Indique que le programme s'est terminé avec succès
}
