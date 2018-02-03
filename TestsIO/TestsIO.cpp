//
// Book : Version du chapitre 10 de l'ouvrage.
// "Programming -- Principles and Practice Using C++" de Bjarne Stroustrup (2ème édition : 2014)
// Commit initial : 31/01/2018 
// Commit en cours : Drill du cours = page 354 à 376
// Caractères spéciaux : [ ]   '\n'   {  }   ||   ~   _     @

// Librairie indispensable sous Windows et VC++
#include "stdafx.h"


// Librairie restreinte fournie par BS
#include "std_lib_facilities.h"

struct Reading{
	string mag;
	string numero;
	string numero_abo;
	string nom_prenom;
	string code_postal;
	string ville;
	string email;
};

struct Point {
	int x;
	int y;
};

void skip_to_int()
{  
	if (cin.fail()) {
		cin.clear();
		for (char ch;cin>>ch;) {
			if (isdigit(ch) || ch=='-') {
				cin.unget();
				return;
			}
		
		}
	
	}

	error("no input");

}


int get_int()
{  
	int n = 0;
	while(true) {
		if (cin >> n) return n;
		cout << "Désolé mais ce n'est pas un nombre : ré-essayez\n ";
		skip_to_int();
	
	
	}
	
}

int get_int(int low, int high)
{  
	cout << "Entrez un entier entre " << low << " et " << high << " inclus :\n";

	while (true) {
		int const n = get_int();
		if (low <= n && n <=high) return n;
		cout << "Sorry " << n << " is not in the [ " << low << ":" << high << "] range; please try again\n";
	
	}

}

int main()
{
	try
	{
		// A mettre dans la 1ère ligne du "main" pour afficher les caractères accentués sous la console Windows
		std::locale::global(std::locale("fr-FR"));
		
		/* Lecture fichier de test : utilisation des techniques du cours
		// ***************************************************************************************************************************************************************************************
		string iname = "Abonnes.prn";
		//string iname = "Abonnes1.txt";
		
		// On ouvre le fichier présent sur le disque dur en spécifiant la gestion d'exception (page 357)
		ifstream ist{ iname };
		ist.exceptions(ist.exceptions() | ios_base::badbit);
		if (!ist)error("Impossible d'ouvrir le fichier ",iname);

		// On crée le vecteur qui va stocker les abonnés
		vector<Reading>abonnes;
		
		static const int taille_max = 125;						// Nombre de caractères max d'un enregistrement
		char str[taille_max]="";								// Cette variable "char" va stocker chaque enregistrement lu
		
		while (!ist.eof()) {									// Tant que la fin du fichier n'est pas atteinte, on continue à extraire de nouveaux enregistrements
			ist.clear();										// On "cleare" le flux SINON la lecture s'arrêt au bout de 38 enregistrements : INCOMPRIS à date
			ist.getline(&str[0], taille_max, '\n');				// On lit un nb de caractères max MAIS on s'arrête dès que l'on rencontre un NEWLINE
			string buffer(str);									// On crée une chaîne pour la manipuler avec 'substr' à partir des caractères récoltés dans le flux istream 
			
			// On créée le vecteur 'champ' destiné à récupérer les informations de la ligne lue dans le fichier, ainsi que son format via des bornes car aucun caractère ne sépare les champs
			vector <string> champ(7); // mag, numero, numero_abo, nom_prenom, code_postal, ville, email
			vector <unsigned int> borne_inf = { 0,4,9,19,58,64,92 };	// ex : le champ mag débute à 0 et se termine à 3 ; le champ email débute à 92 et se termine à 124
			vector <unsigned int> borne_sup = { 3,8,18,57,63,91,124 };	// Quelques bugs non gérées : les codes postaux > 6 caractères (ex : Belgique, Suisse ou Québec parfois)
						
			// On boucle pour récupérer les 7 champs potentiels de la ligne lue du fichier
			for (int i=0;i<7;i++) {
				if (borne_sup[i]< buffer.size()) {		// Tant que la longueur de la chaîne récupérée est supérieure à la borne du champ en cours de recherche, on continue...
					champ[i]=buffer.substr(borne_inf[i], borne_sup[i]- borne_inf[i]+1);
					
				} else {
					champ[i] = buffer.substr(borne_inf[i], buffer.size() - borne_inf[i]);	//... sinon, on comprend que le champ est plus "court", donc on recalcule sa taille max en repartant de la taille de la ligne i.e. buffer.size() 
					break;																	// et on sort de la boucle
				}
			}
						
			// On crée le vecteur résultante, sachant que des champs peuvent être vides (ex : email absent pour le dernier champ)
			abonnes.push_back(Reading{ champ[0] , champ[1] , champ[2] , champ[3] , champ[4] , champ[5] , champ[6] });
			
		}
		
		cout << "Le fichier contient " << abonnes.size() << " enregistrements\n";
		cout << "--------------------------------------------------------------\n";
		keep_window_open();
		for (unsigned int i = 0; i < abonnes.size(); ++i)
			cout << abonnes[i].mag << " - " << abonnes[i].numero << " - " << abonnes[i].numero_abo << " - " << abonnes[i].nom_prenom << " - " << abonnes[i].code_postal << " - " << abonnes[i].ville << " - " << abonnes[i].email << '\n';
		*/
		
		
		// Drill page 376 : utilisation des techniques du cours autour de l'écriture et la lecture en fichier
		// ***************************************************************************************************************************************************************************************
		// Création d'un fichier pour écriture
		string filename = "mydata.txt";
		ofstream ost{ filename };
		ost.exceptions(ost.exceptions() | ios_base::badbit);
		if (!ost)error("Impossible d'ouvrir le fichier ", filename);
		
		vector <Point> original_points;
		
		// Saisie de 7 coordonnées (x,y)
		for (int i=1;i<8;++i) { 		
		
			// Saisie d'entiers représentant les coordonnées
			cout << "Saisie des coordonnées (x,y) du point "<<i<<" :\n";
			int const x = get_int(0, 100);
			int const y = get_int(0, 100);
			
			original_points.push_back(Point{ x,y });	// vecteur populé

		}

		// Affichage des coordonnées saisies et écriture simultanée dans le fichier mydata.txt
		cout << "\nLes 7 coordonnées saisies : ";
		for (auto coord:original_points) {
			cout << "("<<coord.x<<","<<coord.y<<")";
			ost<<  coord.x << " " << coord.y << "\n";
		}
		
		ost.close();	// Fermeture du fichier
		cout << "\n";
		keep_window_open();

		// Ouverture en lecture du fichier mydata.txt, lecture des élements et stockage des données lues dans un nouveau vecteur "processed_points"
		ifstream ist{ filename };
		ist.exceptions(ist.exceptions() | ios_base::badbit);
		if (!ist)error("Impossible d'ouvrir le fichier ", filename);
		
		vector <Point> processed_points;
		int x=0;
		int y=0;
		while (ist >> x >> y) processed_points.push_back(Point{ x,y });	// On lit les valeurs tant qu'il en existe dans le fichier et on les stocke dans le new vector
		ist.close();	// Fermeture du fichier
		
		// Affichage simultané des deux vecteurs
		// -------------------------------------

		// 1ère vérification de la taille des deux vecteurs
		if (original_points.size()!= processed_points.size())error("Les tailles des deux vecteurs sont différentes : corruption de fichier !");

		cout <<"Vecteur originel			Vecteur processé\n";
		for (unsigned int i = 0; i < original_points.size(); ++i) {
			cout <<"("<< original_points[i].x << "," << original_points[i].y <<")"<<"					"<<"(" << processed_points[i].x << "," << processed_points[i].y << ")\n";
			
			// 2ème vérification concernant la consistance des coordonnées
			if(original_points[i].x != processed_points[i].x || original_points[i].y != processed_points[i].y)error("Une coordonnée est différente : corruption de fichier !");
		}
		
		keep_window_open();
		return 0;
	}
	
	/*
	catch (XXXXXXX::Invalid&)
	{
		cerr << "error: ???????????????????? ! \n";
		keep_window_open("q");	// For some Windows(tm) setup
		return 1;
	}
	*/

	catch (runtime_error& e)
	{	// this code is to produce error messages
		cout << e.what() << '\n';
		keep_window_open("q");	// For some Windows(tm) setups
	}
}

