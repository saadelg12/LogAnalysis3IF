/*************************************************************************
                           Statistiques  -  description
                             -------------------
    début                : $06/02/2024$
    copyright            : (C) $2024$ par $mcochet zrjimati agueye selghissas$
    e-mail               : $melisse.cochet@insa-lyon.fr saad.el-ghissassi@insa-lyon.fr$
*************************************************************************/

//-- Réalisation de la classe <Statistiques> (fichier Statistiques.cpp) --

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
#include <iostream>
#include <fstream>
using namespace std;
//------------------------------------------------------ Include personnel
#include "Statistiques.h"

//------------------------------------------------------------- Constantes

//----------------------------------------------------------------- PUBLIC

//----------------------------------------------------- Méthodes publiques
void Statistiques::AjoutLog(string cible, string referer )
{
    Classement[cible]++; // Incrémente le nombre de hits pour la cible et le stocke dans Classement
    Graph[referer][cible] ++; // Incrémente le nombre de hits pour le couple (referer,cible) et le stocke dans Graph
} //----- Fin de AjoutLog       


void Statistiques::GetGraph (const string& filename)
{
    ofstream fic ; // Fichier .dot
    fic.open(filename); // Ouverture du fichier .dot

    if(!fic.is_open()) // Vérification de l'ouverture du fichier
    {
        cerr << "Erreur d’ouverture de " << filename << endl; // Affichage d'un message d'erreur si le fichier n'a pas pu être ouvert
        return;
    }    

    // Redirection de la sortie standard vers le fichier .dot
    streambuf *oldCoutBuffer = cout.rdbuf(fic.rdbuf());

    cout << "digraph {\n"; // Entête du fichier .dot

    // Map pour stocker les identifiants uniques des noeuds
    std::map<std::string, int> nodeIDs; // Associe à chaque noeud un identifiant unique
    int nextID = 0; // Identifiant du prochain noeud

    // Associe à chaque noeud un identifiant unique
    for (const std::pair<const std::string, std::unordered_map<std::string, int>>& pair : Graph) // Parcours de la map Graph
    {
        if(nodeIDs.find(pair.first) == nodeIDs.end()) // Si le noeud n'est pas dejà dans le map
        {
            nodeIDs[pair.first] = nextID++; // On lui associe un identifiant unique
        }
        //for (const std::pair<std::string, int>& inner_pair : pair.second) // Parcours de la map interne
        for (const auto& inner_pair : pair.second)   //mot clé auto utilisé pour éviter le warning généré par la ligne précédente
        //et c'est pour cette même raison qu'on utilise le mot clé auto dans les autres boucles du code
        //Le code fonctionne parfaitement sans ce mot-clé, il y a juste des warning générés lors de la compilation
        {
            if(nodeIDs.find(inner_pair.first) == nodeIDs.end()) // Si le noeud n'est pas déjà dans la map interne 
            {
                nodeIDs[inner_pair.first] = nextID++; // On lui associe un identifiant unique
            }
        }
    }

    // Écriture des noeuds dans le fichier .dot
    //for (const std::pair<std::string, int>& pair : nodeIDs) // Parcours de la map nodeIDs
    for (const auto& pair : nodeIDs)   //mot clé auto utilisé pour éviter le warning
    {
        cout << "  node" << pair.second << " [label=\"" << pair.first << "\"];\n"; // Écriture du noeud avec son identifiant et son label
    }

    // Écriture des arêtes dans le fichier .dot
    //for (const std::pair<const std::string, std::unordered_map<std::string, int>>& pair : Graph) // Parcours de la map Graph
    for (const auto& pair : Graph)   //mot clé auto utilisé pour éviter le warning
    {
        const string& source = pair.first; // Noeud source
        const std::unordered_map<std::string, int>& targets = pair.second; // Noeuds cibles
        

        //for (const std::pair<std::string, int>& target : targets) // Parcours des noeuds cibles
        for (const auto& target : targets)   //mot clé auto utilisé pour éviter le warning
        {
            const string& dest = target.first; // Noeud cible
            int count = target.second; // Nombre de visites
            // Écriture de l'arête avec le label représentant le nombre de visites
            cout << "  node" << nodeIDs[source] << " -> node" << nodeIDs[dest] << " [label=\"" << count << "\"];\n";
        }
    }

    cout << "}\n"; // Fin du fichier .dot

    // Restauration de la sortie standard
    cout.rdbuf(oldCoutBuffer); 

    fic.close(); // Fermeture du fichier .dot

    // Affichage d'informations à l'utilisateur
    cout << "Vous souhaitez produire un fichier au format GraphViz du graphe." << endl ;
    cout << "Chaque document apparaitra sous la forme d'un nœud et chaque arc indiquera le nombre de parcours associes." << endl ;
    cout << "Fichier " << filename << " généré avec succès" << endl;
    cout << "Vous pouvez générer une image avec la commande : dot -Tpng " << filename << " -o [nomImage].png" << endl;
}  // -- Fin de GetGraph

void Statistiques::GetClassement (int &nbr_cibles )
{
    // Affichage
    map <string, int> :: iterator debut,fin; // Itérateurs pour parcourir la map Classement
    debut = Classement.begin(); // Itérateur de début
    fin = Classement.end(); // Itérateur de fin
    
    const int topN = 10; // Nombre de cibles à afficher
    nbr_cibles = 0; // Compteur de cibles affichées

    // Copie temporaire de Classement
    map <string, int> ClassementTmp ( Classement ); // Copie de la map Classement

    while((nbr_cibles < topN) && ( !ClassementTmp.empty() )) // Tant qu'on a pas affiché les topN cibles et que la map temporaire n'est pas vide
    { 
        // On trouve l'élément avec la plus grande valeur
        map<string, int>::iterator maxHit = max_element(
            ClassementTmp.begin(),
            ClassementTmp.end(),
            // Définition d'une fonction de comparaison qui compare le nb de hits
            [](const pair<string, int>& lhs, const pair<string, int>& rhs) {
                return lhs.second < rhs.second;
            }
        );

        // On vérifie si un élément a été trouvé
        if (maxHit != ClassementTmp.end()) 
        {
            // Afficher l'élément avec la plus grande valeur
            cout << nbr_cibles+1 << ". " << maxHit->first << " (" << maxHit->second << " hits)" << endl;

            // On supprime l'élément trouvé de la map temporairement
            ClassementTmp.erase(maxHit);

            // On incrémente le compteur
            ++nbr_cibles;
        }
    }  
    // On supprime tous les éléments de ClassementTmp
    ClassementTmp.clear();

} //----- Fin de GetClassement       


//-------------------------------------------- Constructeurs - destructeur
Statistiques::Statistiques ( )
{
#ifdef MAP
    cout << "Appel au constructeur de <Statistiques>" << endl;
#endif

} //----- Fin de Statistiques


Statistiques::~Statistiques ( )
{
#ifdef MAP
    cout << "Appel au destructeur de <Statistiques>" << endl;
#endif
} //----- Fin de ~Statistiques


//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées

