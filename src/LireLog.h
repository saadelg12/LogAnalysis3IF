/*************************************************************************
                                LireLog.h  
                             -------------------
    début                : $06/02/2024$
    copyright            : (C) $2024$ par $mcochet zrjimati agueye selghissas$
    e-mail               : $melisse.cochet@insa-lyon.fr saad.el-ghissassi@insa-lyon.fr$
*************************************************************************/

//---------- Interface de la classe <LireLog> (fichier LireLog.h) ----------------
#if ! defined ( LireLog_H )
#define LireLog_H
#include <string>
#include <fstream>
#include <iostream>

//--------------------------------------------------- Interfaces utilisées

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//Structure Date qui continent des string
struct Date 
{   
    string jour, mois, annee, heure, minute, seconde, fuseau;
    Date(string j = "", string m = "", string a = "", string h = "", string min = "", string s = "", string f = ""):jour(j),mois(m),annee(a),heure(h),minute(min),seconde(s),fuseau(f){};
    // Constructeur par défaut avec des valeurs par défaut pour chaque composant de la date
};

//Structure Ligne qui représente "un" log : elle contient des string et une date
struct Ligne
{
    string adresseIP, logUser, nomUser, requete, statut, qteDeDonnees, refererant, idClient;
    Date date;
    Ligne(): adresseIP(""),  logUser(""),  nomUser(""), requete(""), statut(""), qteDeDonnees("") , refererant("") , idClient("") , date() {};
    // Constructeur par défaut qui initialise chaque composant d'un log à une chaîne de caractères vide
    // (les valeurs par défaut de la structure Date seront utilisées pour initialiser la date)
    //Reprise des composants d'un log dans l'ordre
};

//------------------------------------------------------------------------
// Rôle de la classe <LireLog>
//LireLog est une spécialisation de ifstream. 
//Elle hérite de ifstream et ajoute des méthodes supplémentaires pour lire et analyser un fichier de log.
//Elle extrait d'un fichier log toutes les informations qui le composent (adresse IP, usernam, statut, referer, date, etc...).
//Elle stocke ces informations dans une structure Ligne qui sera utile afin de générer des statistiques dans le main.
//Elle permet également de vérifier si un fichier est vide ou non, dans quel cas il n'y aura pas d'information à extraire, 
//et donc pas de statistiques à générer.
//
//------------------------------------------------------------------------

class LireLog : public ifstream
{
//----------------------------------------------------------------- PUBLIC

public:
//----------------------------------------------------- Méthodes publiques
    void AfficheLigne(Ligne L);
    // Mode d'emploi :  Affiche une ligne L 
    // Contrat : Un objet de type ligne est exigé

    bool verifie_vide();
    // Mode d'emploi : Vérifie si le fichier associé à l'objet LireLog est vide
    // Contrat : Aucun

    LireLog & ProchainLog(Ligne & log);
    // Mode d'emploi : Lit la prochaine ligne du fichier log et la stocke dans l'objet Ligne passé par référence
    // Contrat : Un objet de type ligne est exigé

    void GetInformations(Ligne L, string &cible, string &referant, int &heure, int &statut, bool &verifie_extensions);
    // Mode d'emploi : Recupère les parties du referer, de la cible, de l'heure d'un hit, du statut de connexion, 
    //                 et les extensions utiles à l'analyse
    //                 à partir d'une ligne
    // Contrat : Un objet de type ligne est exigé
   


//-------------------------------------------- Constructeurs - destructeur

    LireLog (string nomFichier );
    // Mode d'emploi : Constructeur de LireLog permettant l'ouverture d'un flux à partir d'un string
    // Contrat : Aucun
    

    virtual ~LireLog ( );
    // Mode d'emploi : Destructeur de LireLog
    // Contrat : Aucun


//------------------------------------------------------------------ PRIVE

protected:
//----------------------------------------------------- Méthodes protégées

//----------------------------------------------------- Attributs protégés
const string URLdeBase = "http://intranet-if.insa-lyon.fr" ;
// URL de base utilisée dans le traitement des informations extraites
} ;

#endif // LireLog_H
