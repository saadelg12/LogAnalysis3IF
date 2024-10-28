/*************************************************************************
                                LireLog.cpp
                             -------------------
    début                : $06/02/2024$
    copyright            : (C) $2024$ par $mcochet zrjimati agueye selghissas$
    e-mail               : $melisse.cochet@insa-lyon.fr saad.el-ghissassi@insa-lyon.fr$
*************************************************************************/

//---------- Réalisation de la classe <LireLog> (fichier LireLog.cpp) ------------

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
using namespace std;
#include <iostream>

//------------------------------------------------------ Include personnel
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "LireLog.h"

//------------------------------------------------------------- Constantes

const char SEP = ' '; // Séparateur utilisé dans le fichier log correspondant aux espaces
const char SEP2 = '"';  // Séparateur utilisé dans le fichier log
const char SEPdate1 = '/'; // Séparateur de date
const char SEPdate2 = ':'; // Séparateur d'heure
const string Extension[12] = {"js","css","php","jpeg","jpg","svg","tiff","webp","ico","png","bmp","gif"}; //Tableau des extensions possibles
const int sizeExtension = 12;  // Nombre d'extensions possibles

//----------------------------------------------------------------- PUBLIC

//----------------------------------------------------- Méthodes publiques
void LireLog::AfficheLigne(Ligne L)
{
    cout<<L.adresseIP<<' '<<L.logUser<<' '<<L.nomUser<<" ["<<L.date.jour<<'/'<<L.date.mois<<'/'<<L.date.annee<<":"<<L.date.heure<<':'<<L.date.minute<<':'<<L.date.seconde<<' '<<L.date.fuseau<<']';
    cout<<' '<<L.requete<<' '<<L.statut<<' '<<L.qteDeDonnees<<' '<<L.refererant<<' '<<L.idClient<<endl;
} //----- Fin de AfficheLigne

bool LireLog::verifie_vide()
{
    // Se déplace à la fin du fichier pour vérifier la taille
    this->seekg(0, std::ios::end);
    
    // Si la taille est de 0, le fichier est vide
    if(this->tellg()==0){
        return true;
    }

    // Sinon, revient au début du fichier
    this->seekg(0, std::ios::beg);
    return false;
} //----- Fin de verifie_vide

LireLog & LireLog::ProchainLog (Ligne & log)
{   
    // Extraction des informations de la ligne
    std :: getline(*this,log.adresseIP,SEP); // Extraction de l'adresse IP
    std :: getline(*this,log.logUser,SEP); // Extraction du logUser
    std :: getline(*this,log.nomUser,SEP); // Extraction du nomUser

    seekg(1 , ios::cur); // Saut d'un caractère puis extraction de la date

    std :: getline(*this,log.date.jour,SEPdate1); 
    std :: getline(*this,log.date.mois,SEPdate1); 
    std :: getline(*this,log.date.annee,SEPdate2);
    std :: getline(*this,log.date.heure,SEPdate2);
    std :: getline(*this,log.date.minute,SEPdate2);
    std :: getline(*this,log.date.seconde,SEP);
    std :: getline(*this,log.date.fuseau,']');

    seekg(2 , ios::cur); // Saut d'un caractère

    std ::  getline(*this,log.requete,SEP2); // Extraction de la requête
    seekg(1 , ios::cur); // Saut d'un caractère

    std ::  getline(*this,log.statut,SEP); // Extraction du statut
    std ::  getline(*this,log.qteDeDonnees,SEP); // Extraction de la quantité de données

    seekg(1 , ios::cur); // Saut d'un caractère
    std ::  getline(*this,log.refererant,SEP2); // Extraction du refererant
    std ::  getline(*this,log.idClient); // Extraction de l'idClient

    return *this;
} //----- Fin de ProchainLog


void LireLog::GetInformations(Ligne L, string &cible, string &referant, int &heure, int &statut, bool &verifie_extensions)
{
    // Extraction de la cible
    size_t pos1 = L.requete.find(" "); // Recherche de la première occurence d'un espace
    size_t pos2 = L.requete.find(" ", pos1 + 1); // Recherche de la deuxième occurence d'un espace
    if (pos1 != string::npos && pos2 != string::npos) // Si les deux occurences existent
    {
        cible = L.requete.substr(pos1 + 1, pos2 - pos1 - 1); // Extraction de la cible
    
        // Filtrage de la cible dans le cas d'un "?"
        size_t pos3 = cible.find("?"); // Recherche de la première occurence d'un "?"
        if (pos3 != string::npos) // Si l'occurence existe
        {
            cible = cible.substr(0, pos3); // Extraction de la cible jusqu'au "?"
        }
        // Filtrage de la cible dans le cas d'un ";"
        size_t pos4 = cible.find(";"); // Recherche de la première occurence d'un ";"
        if (pos4 != string::npos) // Si l'occurence existe
        {
            cible = cible.substr(0, pos4); // Extraction de la cible jusqur au ";"
        }
    }
    else // Si il n'y a pas de cible
    {
        cible = ""; // La cible est vide
    }

    // Extraction du referant
    referant = L.refererant; // Extraction du referant
    if(referant.find(URLdeBase) != string::npos) // Si le referant contient l'URL de base
    { 
        referant = referant.substr(URLdeBase.size()); // Supprime l'URL de base
    } 
    else 
    {
        // Filtrage en cas d'un '?' (raccourcissant le referant)
        size_t pos = referant.find("?"); // Recherche de la première occurence d'un "?"
        if(pos != string::npos) // Si l'occurence existe
        {
            referant = referant.substr(0, pos); // Extraction du referant jusqu'au "?"
        }

        // Filtrage en cas d'un ';' (raccourcissant le referant)
        pos = referant.find(";"); // Recherche de la première occurence d'un ";"
        if(pos != string::npos) // Si l'occurence existe
        {
            referant = referant.substr(0, pos); // Extraction du referant jusqu'au ";"
        }
    }

    // Extraction de l'heure
    heure = stoi(L.date.heure);

    // Extraction du statut
    statut = stoi(L.statut);

    // Vérification de l'extension
    verifie_extensions = false;
    for (int i = 0; i < sizeExtension; i++) // Parcours du tableau des extensions possibles
    {
        
        // On vérifie si l'extension figure dans le tableau des extensions possibles
        if (cible.find(Extension[i]) != string::npos)
        {
            verifie_extensions = true; // Si oui, on met le booléen à vrai
            break; // On sort de la boucle
        }
    }
} //----- Fin de GetInformations

//-------------------------------------------- Constructeurs - destructeur

LireLog::LireLog (string nomFichier) : ifstream(nomFichier, ios::in)
{      
#ifdef MAP
    cout << "Appel au constructeur de <LireLog>" << endl;
#endif
} //----- Fin de LireLog

LireLog::~LireLog ()
{
#ifdef MAP
    cout << "Appel au destructeur de <LireLog>" << endl;
#endif
} //----- Fin de ~LireLog

//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées
