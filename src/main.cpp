/*************************************************************************
                                Main.cpp  
                             -------------------
    début                : $06/02/2024$
    copyright            : (C) $2024$ par $mcochet zrjimati agueye selghissas$
    e-mail               : $melisse.cochet@insa-lyon.fr saad.el-ghissassi@insa-lyon.fr$
*************************************************************************/
//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
#include <iostream>
#include <map>
#include <unistd.h>
#include <string>
#include <vector>
using namespace std;

//-------------------------------------------------------- Include personnel
#include "LireLog.h"
#include "Statistiques.h"

//----------------------------------------------------------------- PUBLIC

//----------------------------------------------------- Méthodes publiques

int main(int argc, char** argv) 
{

  // Dans cette première partie du code, on vérifie que les options entrées en argument sont valides

    if(argc == 1) 
    // Si aucun argument n'est entré, on affiche un message d'erreur et on quitte le programme
    // On ne peut pas générer de statistiques sans fichier.log
    {
      cerr<<"Erreur : pas assez d'arguments." <<endl;
      cerr<<"Vous pouvez générer l'aide en spécifiant uniquement l'option -h dans la commande : "<<endl;
      cerr << "./analog -h" << endl;
      return 1;
    }
    
    // On stocke les options dans un vecteur
    //Rappel des options : 
    // -g fichier.dot : génère un fichier .dot
    // -t horaire : filtre les logs pour un horaire donnée
    // -e : exclut les requêtes avec extensions
    // -s : filtre les logs pour les requêtes ayant un statut 200
    // -h : affiche l'aide
    vector <string> option;
    
    for (int i = 1 ; i < argc  ; i++) // On parcourt les options en argument
    {
      option.push_back(argv[i]); // On stocke les options dans le vecteur
    }
    vector<string>::iterator it ; // Itérateur pour parcourir le vecteur option
    int g =0,e = 0,t =0, num=0, horaire=0, dot=0; // Variables pour compter le nombre d'occurences de chaque option
    int size_top = 10; // Variable pour stocker la taille du classement

    if(option.size()!=0) // On vérifie que le vecteur option n'est pas vide
    {
      it=option.begin(); // On initialise l'itérateur
      while(it!=option.end()) // On parcourt le vecteur option
      {
          if (*(it) == "-h") // Si l'option est -h
          {
            if(option.size()>1) // On vérifie que l'option n'est pas suivie d'autres arguments
            {
              cerr << "Erreur : l'option -h n'a pas été entrée dans un format valide"<<endl;
              cerr << "Assurez-vous d'avoir saisi la bonne syntaxe et de n'avoir spécifié aucune autre option :" <<endl;
              cerr<<"./analog -h"<<endl;
              return 1;
            }
            else
            {
              cout << "Bienvenue dans l'aide de notre analyseur de logs" << endl;
              cout << "Pour générer des statistiques à partir d'un fichier.log, veuillez saisir la commande suivante :" << endl;
              cout << "./analog [option] nomFichier.log" << endl;
              cout << "L'indication d'options est facultative et dans tous les cas, les 10 documents les plus consultés seront affichés" << endl;
              cout << "L'ordre d'entrée des options n'importe pas" << endl;
              cout << "Les options disponibles sont :" << endl;
              cout << "-g fichier.dot : génère un fichier .dot à partir du classement" << endl;
              cout << "-t entier : filtre les documents les plus visités sur une plage horaire [entier;entier+1] où entier est une heure comprise entre 0 et 23" << endl;
              cout << "-e : filtre les documents qui ont une extension de type image, css ou javascript." << endl;
              cout << "Vous pouvez utiliser plusieurs options de filtrage simultanément" << endl;
              cout << "Vous pouvez revenir sur le menu d'aide en spécifiant uniquement l'option -h dans la commande : " << endl;
              cout << "./analog -h" << endl;
              return 0;
            }
          }

          if(*(it) == "-g") // Si l'option est -g
          {
            g++; // On incrémente le compteur
            if(num==static_cast<int>(option.size())-1 || g>1) 
            // On vérifie que l'option n'est pas le dernier argument et qu'elle n'est pas utilisée plusieurs fois
            { 
              cerr << "Erreur : l'option -g n'a pas été entrée dans un format valide"<<endl;
              cerr << "Assurez-vous d'avoir saisi la bonne syntaxe :" <<endl;
              cerr<<"./analog [-g fichier.dot] [option] nomFichier.log"<<endl;
              cerr<<"Vous pouvez générer l'aide en spécifiant uniquement l'option -h dans la commande : "<<endl;
              cerr << "./analog -h" << endl;
              return 1;
            }
            else
            {
              if (option[num+1].size() >= 4) 
              {
                string end = option[num+1].substr(option[num+1].size() - 4);
              // substr(option[num+1].size() - 4) : Extrait les 4 derniers caractères de option[num+1] (la taille de .dot est 4). 
              // Ensuite, end != ".dot" vérifie si ces 4 derniers caractères ne sont pas .dot et affiche un message d'erreur le cas échéant
              if (end != ".dot") 
              {
                cerr << "Erreur : le fichier .dot n'a pas été entré dans un format valide"<<endl;
                cerr << "Assurez-vous d'avoir saisi la bonne syntaxe :" <<endl;
                cerr<<"./analog [-g fichier.dot] [autres options] nomFichier.log"<<endl;
                cerr<<"Il faut impérativement préciser l'extension .dot dans le nom du graphe à générer."<<endl;
                cerr<<"Vous pouvez générer l'aide en spécifiant uniquement l'option -h dans la commande : "<<endl;
                cerr << "./analog -h" << endl;
                return 1;
              }
              dot=num+1; // On stocke l'indice du fichier .dot
              }
              else // Si l'entrée après -g n'est pas un fichier .dot (par exemple si l'utilisateur a saisi -g -g)
              {
                cerr << "Erreur : l'option -g n'a pas été entrée dans un format valide"<<endl;
                cerr << "Assurez-vous d'avoir saisi la bonne syntaxe :" <<endl;
                cerr<<"./analog [-g fichier.dot] [option] nomFichier.log"<<endl;
                cerr<<"Vous pouvez générer l'aide en spécifiant uniquement l'option -h dans la commande : "<<endl;
                cerr << "./analog -h" << endl;
                return 1;
              }
            }
          }

          if(*(it) == "-t") // Si l'option est -t
          {
            t++; // On incrémente le compteur
            if(num==static_cast<int>(option.size())-1 || t>1)
            // On vérifie que l'option n'est pas le dernier argument et qu'elle n'est pas utilisée plusieurs fois
            {
              cerr << "Erreur : l'option -t n'a pas été entrée dans un format valide"<<endl;
              cerr << "Assurez-vous d'avoir saisi la bonne syntaxe :" <<endl;
              cerr<<"./analog [-t horaire] [autres options] nomFichier.log"<<endl;
              cerr<<"Vous pouvez générer l'aide en spécifiant uniquement l'option -h dans la commande : "<<endl;
              cerr << "./analog -h" << endl;
              return 1;
            }
            if(option[num+1].find_first_not_of("0123456789") != string::npos )
            // On vérifie que l'option est suivie d'un entier
            { 
              cerr << "Erreur : l'horaire n'est pas valide, il doit être un entier"<<endl;
              cerr<<"Vous pouvez générer l'aide en spécifiant uniquement l'option -h dans la commande : "<<endl;
              cerr << "./analog -h" << endl;
              return 1;
            } 
            if(stoi(option[num+1])>23 || stoi(option[num+1])<0)
            // On vérifie que la plage horaire est correcte
            { 
              cerr << "Erreur : l'horaire doit etre compris entre 0 et 23"<<endl;
              cerr<<"Vous pouvez générer l'aide en spécifiant uniquement l'option -h dans la commande : "<<endl;
              cerr << "./analog -h" << endl;
              return 1;
            } 
            horaire=num+1; // On stocke l'indice de l'horaire
          }

          if(*(it) == "-e")// Si l'option est -e
          {
            e++; // On incrémente le compteur
            if(num==static_cast<int>(option.size())-1 || e>1)
            // On vérifie que l'option n'est pas le dernier argument et qu'elle n'est pas utilisée plusieurs fois
            {
              cerr << "Erreur : l'option -e n'a pas été entrée dans un format valide"<<endl;
              cerr << "Assurez-vous d'avoir saisi la bonne syntaxe :" <<endl;
              cerr<<"./analog [-e] [autres options] nomFichier.log"<<endl;
              cerr<<"Vous pouvez générer l'aide en spécifiant uniquement l'option -h dans la commande : "<<endl;
              cerr << "./analog -h" << endl;
              return 1;
            }
          }
          num++; // On incrémente le compteur
          it++; // On incrémente l'itérateur
      }  
    }
  
    // Dans cette partie du code, on génère les statistiques à partir du fichier.log
      string testLog = (string)argv[argc-1]; // On récupère le fichier.log
      if(testLog.find(".log")!= string::npos) // On vérifie que le fichier.log est bien entré
      {  
        LireLog log(argv[argc - 1]); // On crée un objet de type LireLog a partir du nom du fichier.log
        Ligne ligne; // On crée un objet de type Ligne
        Statistiques mes_stats; // On crée un objet de type Statistiques
        if(!log) // Test de l'ouverture du fichier
        { 
          cerr<<"Erreur : le fichier \".log\" est inaccessible ou introuvable.";
          cerr<<"Assurez-vous d'avoir saisi le bon nom de fichier."<<endl;
          cerr<<"Vous pouvez générer l'aide en spécifiant uniquement l'option -h dans la commande : "<<endl;
          cerr << "./analog -h" << endl;
          return 1;
        }
        if(log.verifie_vide())
        {
          cerr<<"Erreur : le fichier.log est vide."<<endl;
          cerr<<"Il n'y a pas de statistiques à générer."<<endl;
          cerr<<"Vous pouvez générer l'aide en spécifiant uniquement l'option -h dans la commande : "<<endl;
          cerr << "./analog -h" << endl;
          return 1;
        }

      // Cas courant, lorsque l'utilisateur n'a spécifié aucune option
        if (e == 0 && t == 0 && g == 0)
        {
          int nbr_cibles = 0; // On crée une variable pour compter le nombre de cibles
          while(log.ProchainLog(ligne)) // On parcourt le fichier.log
          {
            string cible, referant; // On crée des variables pour stocker la cible et le referant
            int heure, statut; // On crée des variables pour stocker l'heure et le statut
            bool verifie_extensions; // On crée une variable pour vérifier les extensions
            log.GetInformations(ligne, cible, referant, heure, statut, verifie_extensions); // On récupère les informations de chaque log
            //log.AfficheLigne(ligne); // On affiche les informations de chaque log (pour vérification et maintenance)
            mes_stats.AjoutLog(cible, referant); // On ajoute les informations à l'objet de type Statistiques
          }
          cout << "Vous désirez produire un top " << size_top << " des documents les plus consultés." << endl;
          cout << "Le classement des documents les plus consultés par ordre décroissant de popularité est : " << endl;
          mes_stats.GetClassement(nbr_cibles); // On trie et affiche le classement
          if (nbr_cibles < size_top)
              {
                cout << "Attention, le nombre de cibles présentes dans les logs analysés est inférieur à la taille du classement désirée."<<endl;
                cout << "Seuls "<< nbr_cibles << " documents ont donc été classés."<<endl;
              }
          cout << "N.B : Le classement peut être différent étant donné que plusieurs cibles peuvent avoir le même nombre de Hits." << endl;
        }

      // Cas où seule l'option -e est entrée
        if(e != 0 && t == 0 && g == 0)
        {
          int nbr_cibles = 0; // On crée une variable pour compter le nombre de logs
          while(log.ProchainLog(ligne)) //On parcourt le fichier log
          {
            string cible, referant; // On crée des variables pour stocker la cible et le referant
            int heure, statut; // On crée des variables pour stocker l'heure et le statut
            bool verifie_extensions; // On crée une variable pour vérifier les extensions
            log.GetInformations(ligne, cible, referant, heure, statut, verifie_extensions); // On récupère les informations de chaque log
            if(!verifie_extensions) 
            {
              mes_stats.AjoutLog(cible, referant);
            }
          }
          cout << "Vous désirez produire un top " << size_top << " des documents les plus consultés." << endl;
          cout << "Vous souhaitez exclure tous les documents qui ont une extension de type image, css ou javascript." << endl;
          cout << "Le classement des documents les plus consultés par ordre décroissant de popularité est : " << endl;
          mes_stats.GetClassement(nbr_cibles); // On trie et affiche le classement
          if(nbr_cibles == 0)
          {
              cout << "PAS DE CLASSEMENT ! Le filtrage a produit un résultat nul ! "<<endl;
              return 1 ;
          } 
          else if (nbr_cibles < size_top)
              {
                cout << "Attention, le nombre de cibles présentes dans les logs analysés est inférieur à la taille du classement désirée."<<endl;
                cout << "Seuls "<< nbr_cibles << " documents ont donc été classés."<<endl;
              }
          cout << "N.B : Le classement peut être différent étant donné que plusieurs cibles peuvent avoir le même nombre de Hits." << endl;
        }
        

      // Cas où seule l'option -t est entrée
        if( e== 0 && t != 0 && g == 0)
        {
          int nbr_cibles = 0; // On crée une variable pour compter le nombre de logs
          while(log.ProchainLog(ligne)) // On parcourt le fichier log
          {
            string cible, referant; // On crée des variables pour stocker la cible et le referant
            int heure, statut; // On crée des variables pour stocker l'heure et le statut
            bool verifie_extensions; // On crée une variable pour vérifier les extensions
            log.GetInformations(ligne, cible, referant, heure, statut, verifie_extensions); // On récupère les informations de chaque log    
            if(heure >= stoi(option[horaire]) && heure < stoi(option[horaire]) +1)
            {
              mes_stats.AjoutLog(cible, referant);
            }  
          }
          cout << "Vous désirez produire un top " << size_top << " des documents les plus consultés." << endl;
          cout << "Vous souhaitez sélectionner tous les hits effectués entre " << stoi(option[horaire]) << " et " << stoi(option[horaire]) +1 << " heures" << endl;
          cout << "Le classement des documents les plus consultés par ordre décroissant de popularité est : " << endl;
          mes_stats.GetClassement(nbr_cibles); // On trie et affiche le classement
          if(nbr_cibles == 0)
          {
              cout << "PAS DE CLASSEMENT ! Le filtrage a produit un résultat nul ! "<<endl;
              return 1 ;
          } 
          else if (nbr_cibles < size_top)
              {
                cout << "Attention, le nombre de cibles présentes dans les logs analysés est inférieur à la taille du classement désirée."<<endl;
                cout << "Seuls "<< nbr_cibles << " documents ont donc été classés."<<endl;
              }
          cout << "N.B : Le classement peut être différent étant donné que plusieurs cibles peuvent avoir le même nombre de Hits." << endl;
        }
      
      // Cas où seule l'option -g est entrée
        if( e== 0 && t == 0 && g != 0)
        {
          int nbr_cibles = 0; // On crée une variable pour compter le nombre de logs
          while(log.ProchainLog(ligne)) // On parcourt le fichier log
          {
            string cible, referant; // On crée des variables pour stocker la cible et le referant
            int heure, statut; // On crée des variables pour stocker l'heure et le statut
            bool verifie_extensions; // On crée une variable pour vérifier les extensions
            log.GetInformations(ligne, cible, referant, heure, statut, verifie_extensions); // On récupère les informations de chaque log    
            mes_stats.AjoutLog(cible, referant);
            nbr_cibles++;
          }
          cout << "Vous désirez produire un top " << size_top << " des documents les plus consultés." << endl;
          cout << "Le classement des documents les plus consultés par ordre décroissant de popularité est : " << endl;
          mes_stats.GetClassement(nbr_cibles); // On trie et affiche le classement
          if(nbr_cibles == 0)
          {
              cout << "PAS DE CLASSEMENT ! Le filtrage a produit un résultat nul ! "<<endl;
              cout << "Aucun graphe ne pourra être généré." << endl;
              return 1 ;
          } 
          else if (nbr_cibles < size_top)
              {
                cout << "Attention, le nombre de cibles présentes dans les logs analysés est inférieur à la taille du classement désirée."<<endl;
                cout << "Seuls "<< nbr_cibles << " documents ont donc été classés."<<endl;
              }
          cout << "N.B : Le classement peut être différent étant donné que plusieurs cibles peuvent avoir le même nombre de Hits." << endl;
          
          string dotFileName = option[dot]; // On vérifie si un fichier .dot du même nom existe déjà
          if (access(dotFileName.c_str(), F_OK) != -1) 
            {
              cout << "Attention, le fichier .dot existe deja !\n";
              cout << "Tapez 1 si vous voulez écraser les données du fichier.\n";
              cout << "Tapez tout autre caractère si vous voulez abandonner l'écriture.\n";

              int choice;
              cin >> choice;

              if (choice != 1) 
              {
                cout << "Abandon de l'écriture dans le fichier .dot\n";
                return 1; // ou tout autre code pour arrêter l'exécution
              }
              // Si l'utilisateur a choisi 1, le code continue et écrasera le fichier .dot existant
            }

          mes_stats.GetGraph(option[dot]);
        }
        

      // Cas où les options -e -t sont entrées
        if(e != 0 && t != 0 && g == 0)
        {
          int nbr_cibles = 0 ; // On crée une variable pour compter le nombre de logs
          while(log.ProchainLog(ligne)) // On parcourt le fichier log
          {
            string cible, referant; // On crée des variables pour stocker la cible et le referant
            int heure, statut; // On crée des variables pour stocker l'heure et le statut
            bool verifie_extensions; // On crée une variable pour vérifier les extensions
            log.GetInformations(ligne, cible, referant, heure, statut, verifie_extensions); // On récupère les informations de chaque log
            if(!verifie_extensions && heure >= stoi(option[horaire]) && heure < stoi(option[horaire]) +1) 
            {
              mes_stats.AjoutLog(cible, referant);
            }
          }
          cout << "Vous désirez produire un top " << size_top << " des documents les plus consultés." << endl;
          cout << "Vous souhaitez exclure tous les documents qui ont une extension de type image, css ou javascript." << endl;
          cout << "Vous souhaitez sélectionner tous les hits effectués entre " << stoi(option[horaire]) << " et " << stoi(option[horaire]) +1 << " heures" << endl;
          cout << "Le classement des documents les plus consultés par ordre décroissant de popularité est : " << endl;
          mes_stats.GetClassement(nbr_cibles); // On trie et affiche le classement
          if(nbr_cibles == 0)
          {
              cout << "PAS DE CLASSEMENT ! Le filtrage a produit un résultat nul ! "<<endl;
              return 1 ; 
          } 
          else if (nbr_cibles < size_top)
              {
                cout << "Attention, le nombre de cibles présentes dans les logs analysés est inférieur à la taille du classement désirée."<<endl;
                cout << "Seuls "<< nbr_cibles << " documents ont donc été classés."<<endl;
              }
          cout << "N.B : Le classement peut être différent étant donné que plusieurs cibles peuvent avoir le même nombre de Hits." << endl;
        }

      // Cas où les options -e -g sont entrées
        if(e != 0 && t == 0 && g != 0)
        {
          int nbr_cibles = 0 ; // On crée une variable pour compter le nombre de logs
          while(log.ProchainLog(ligne)) // On parcourt le fichier log
          {
            string cible, referant; // On crée des variables pour stocker la cible et le referant
            int heure, statut; // On crée des variables pour stocker l'heure et le statut
            bool verifie_extensions; // On crée une variable pour vérifier les extensions
            log.GetInformations(ligne, cible, referant, heure, statut, verifie_extensions); // On récupère les informations de chaque log
            if(!verifie_extensions) 
            {
              mes_stats.AjoutLog(cible, referant);
              nbr_cibles++;
            }
          }
          cout << "Vous désirez produire un top " << size_top << " des documents les plus consultés." << endl;
          cout << "Vous souhaitez exclure tous les documents qui ont une extension de type image, css ou javascript." << endl;
          cout << "Le classement des documents les plus consultés par ordre décroissant de popularité est : " << endl;
          mes_stats.GetClassement(nbr_cibles); // On trie et affiche le classement
          if(nbr_cibles == 0)
          {
              cout << "PAS DE CLASSEMENT ! Le filtrage a produit un résultat nul ! "<<endl;
              cout << "Aucun graphe ne pourra être généré." << endl;
              return 1 ;
          } 
          else if (nbr_cibles < size_top)
              {
                cout << "Attention, le nombre de cibles présentes dans les logs analysés est inférieur à la taille du classement désirée."<<endl;
                cout << "Seul "<< nbr_cibles << " documents ont donc été classés."<<endl;
              }
          cout << "N.B : Le classement peut être différent étant donné que plusieurs cibles peuvent avoir le même nombre de Hits." << endl;

          string dotFileName = option[dot]; // On vérifie si un fichier .dot du même nom existe déjà
          if (access(dotFileName.c_str(), F_OK) != -1) 
            {
              cout << "Attention, le fichier .dot existe deja !\n";
              cout << "Tapez 1 si vous voulez écraser les données du fichier.\n";
              cout << "Tapez tout autre caractère si vous voulez abandonner l'écriture.\n";

              int choice;
              cin >> choice;

              if (choice != 1) 
              {
                cout << "Abandon de l'écriture dans le fichier .dot\n";
                return 1; // Ou tout autre code pour arrêter l'exécution
              }
              // Si l'utilisateur a choisi 1, le code continue et écrasera le fichier .dot existant
            }

          mes_stats.GetGraph(option[dot]);
          }
        

      // Cas où les options -t -g sont entrées
        if( e== 0 && t != 0 && g != 0)
        {
          int nbr_cibles = 0 ; // On crée une variable pour compter le nombre de logs
          while(log.ProchainLog(ligne)) //On parcourt le fichier log
          {
            string cible, referant; // On crée des variables pour stocker la cible et le referant
            int heure, statut; // On crée des variables pour stocker l'heure et le statut
            bool verifie_extensions; // On crée une variable pour vérifier les extensions
            log.GetInformations(ligne, cible, referant, heure, statut, verifie_extensions); // On récupère les informations de chaque log    
            if(heure >= stoi(option[horaire]) && heure < stoi(option[horaire]) +1)
            {
              mes_stats.AjoutLog(cible, referant);
            }  
          }
          cout << "Vous désirez produire un top " << size_top << " des documents les plus consultés." << endl;
          cout << "Vous souhaitez sélectionner tous les hits effectués entre " << stoi(option[horaire]) << " et " << stoi(option[horaire]) +1 << " heures" << endl;
          cout << "Le classement des documents les plus consultés par ordre décroissant de popularité est : " << endl;
          mes_stats.GetClassement(nbr_cibles); // On trie et affiche le classement
          if(nbr_cibles == 0)
          {
              cout << "PAS DE CLASSEMENT ! Le filtrage a produit un résultat nul ! "<<endl;
              cout << "Aucun graphe ne pourra être généré." << endl;
              return 1 ;
          } 
          else if (nbr_cibles < size_top)
              {
                cout << "Attention, le nombre de cibles présentes dans les logs analysés est inférieur à la taille du classement désirée."<<endl;
                cout << "Seuls "<< nbr_cibles << " documents ont donc été classés."<<endl;
              }
          cout << "N.B : Le classement peut être différent étant donné que plusieurs cibles peuvent avoir le même nombre de Hits." << endl;
          
          string dotFileName = option[dot]; // On vérifie si un fichier .dot du même nom existe déjà
          if (access(dotFileName.c_str(), F_OK) != -1) 
            {
              cout << "Attention, le fichier .dot existe deja !\n";
              cout << "Tapez 1 si vous voulez écraser les données du fichier.\n";
              cout << "Tapez tout autre caractère si vous voulez abondanner l'écriture.\n";

              int choice;
              cin >> choice;

              if (choice != 1) 
              {
                cout << "Abandon de l'écriture dans le fichier .dot\n";
                return 1; // ou tout autre code pour arrêter l'exécution
              }
              // Si l'utilisateur a choisi 1, le code continue et écrasera le fichier .dot existant
            }

          mes_stats.GetGraph(option[dot]);
          }

        // Cas où les options -e -t -g sont entrées
        if( e!= 0 && t != 0 && g != 0)
        {
          int nbr_cibles = 0; // On crée une variable pour compter le nombre de logs
          while(log.ProchainLog(ligne)) //On parcourt le fichier log
          {
            string cible, referant; // On crée des variables pour stocker la cible et le referant
            int heure, statut; // On crée des variables pour stocker l'heure et le statut
            bool verifie_extensions; // On crée une variable pour vérifier les extensions
            log.GetInformations(ligne, cible, referant, heure, statut, verifie_extensions); // On récupère les informations de chaque log    
            if(!verifie_extensions && heure >= stoi(option[horaire]) && heure < stoi(option[horaire]) +1)
            {
              mes_stats.AjoutLog(cible, referant);
            }  
          }
          cout << "Vous désirez produire un top " << size_top << " des documents les plus consultés." << endl;
          cout << "Vous souhaitez exclure tous les documents qui ont une extension de type image, css ou javascript." << endl;
          cout << "Vous souhaitez sélectionner tous les hits effectués entre " << stoi(option[horaire]) << " et " << stoi(option[horaire]) +1 << " heures" << endl;
          cout << "Le classement des documents les plus consultés par ordre décroissant de popularité est : " << endl;
          mes_stats.GetClassement(nbr_cibles); // On trie et affiche le classement
          if(nbr_cibles == 0)
          {
              cout << "PAS DE CLASSEMENT ! Le filtrage a produit un résultat nul ! "<<endl;
              cout << "Aucun graphe ne pourra être généré." << endl;
              return 1 ;
          } 
          else if (nbr_cibles < size_top)
              {
                cout << "Attention, le nombre de cibles présentes dans les logs analysés est inférieur à la taille du classement désirée."<<endl;
                cout << "Seuls "<< nbr_cibles << " documents ont donc été classés."<<endl;
              }
          cout << "N.B : Le classement peut être différent étant donné que plusieurs cibles peuvent avoir le même nombre de Hits." << endl;
          
          string dotFileName = option[dot]; 
          // On vérifie si un fichier .dot du même nom existe déjà
          if (access(dotFileName.c_str(), F_OK) != -1) 
            {
              cout << "Attention, le fichier .dot existe deja !\n";
              cout << "Tapez 1 si vous voulez écraser les données du fichier.\n";
              cout << "Tapez tout autre caractère si vous voulez abandonner l'écriture.\n";

              int choice;
              cin >> choice;

              if (choice != 1) 
              {
                cout << "Abandon de l'écriture dans le fichier .dot\n";
                return 1; // ou tout autre code pour arrêter l'exécution
              }
              // Si l'utilisateur a choisi 1, le code continue et écrasera le fichier .dot existant
            }
          
          mes_stats.GetGraph(option[dot]);
          }
    return 0 ;
    }
    else
    {
      cerr << "Erreur : il faut entrer un fichier au format .log" << endl;
      cerr<<"Vous pouvez générer l'aide en spécifiant uniquement l'option -h dans la commande : "<<endl;
      cerr << "./analog -h" << endl;
      return 1;
    }
  }// Fin du main