/*************************************************************************
                           Statistiques  -  description
                             -------------------
    début                : $06/02/2024$
    copyright            : (C) $2024$ par $mcochet zrjimati agueye selghissas$
    e-mail               : $melisse.cochet@insa-lyon.fr saad.el-ghissassi@insa-lyon.fr$
*************************************************************************/

//---------- Interface de la classe <Statistiques> (fichier Statistiques.h) ----------------
#if ! defined ( Statistiques_H )
#define Statistiques_H
#include <string>
#include <unordered_map>
#include <map>
#include <utility>
#include <algorithm>
//--------------------------------------------------- Interfaces utilisées

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//------------------------------------------------------------------------
// Rôle de la classe <Statistiques>
// Cette classe gère les statistiques liées à un fichier log.
// Elle génère un classement des cibles les plus fréquemment demandées
// et génère un graphique représentant les requêtes effectuées
//------------------------------------------------------------------------

class Statistiques 
{
//----------------------------------------------------------------- PUBLIC

public:
//----------------------------------------------------- Méthodes publiques
    void AjoutLog (string cible, string referer );
    // Mode d'emploi : Rajoute une paire (cible,referer) au Classement et au Graph 
    // Contrat : Aucun
   
    void GetGraph (const string& filename);
    // Mode d'emploi : Crée ou remplace le fichier .dot  
    // Contrat : Le fichier est un .dot


    void GetClassement (int &nbCibles);
    // Mode d'emploi : Trie et affiche le classement  
    // Contrat :  Aucun


//-------------------------------------------- Constructeurs - destructeur    
    Statistiques ( );
    // Mode d'emploi : Constructeur de la classe Statistiques
    // Contrat : Aucun
    

    virtual ~Statistiques ( );
    // Mode d'emploi : Destructeur de la classe Statistiques
    // Contrat : Aucun
    

//------------------------------------------------------------------ PRIVE

protected:
//----------------------------------------------------- Méthodes protégées

//----------------------------------------------------- Attributs protégés
    unordered_map  <string, unordered_map <string, int > > Graph; // <cible, <referer, nbHits >>
    map <string, int> Classement; // <cible, nbHits>
};

#endif // Statistiques_H

