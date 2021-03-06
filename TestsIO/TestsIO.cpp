//
// Book : Version du chapitre 10 de l'ouvrage.
// "Programming -- Principles and Practice Using C++" de Bjarne Stroustrup (2ème édition : 2014)
// Commit initial : 10/02/2018 
// Commit en cours : 1. Drill du cours page 376 et tests personnels (fichier adresse)    2. Exercices page 377 et 378	3. Exercice 11 page 378
// Caractères spéciaux : [ ]   '\n'   {  }   ||   ~   _     @

// Librairie indispensable sous Windows et VC++
#include "stdafx.h"


// Librairie restreinte fournie par BS
#include "std_lib_facilities.h"
#include <functional>		// Pour pouvoir utiliser bind()

struct temperature {
	int hour;
	double temp;
};

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


bool isInteger_v2(const string s) noexcept {
	// test : has_only_digits ?
	return s.find_first_not_of("0123456789") == string::npos;
}

double moyenne_arithmetique(vector<int>& v)
{
	double moy = 0;
	for (auto i : v) { moy += i; }
	return moy / v.size();
}

double mediane(vector<int>& v)
{
	// Pour déterminer une médiane d'un ensemble de valeurs, il suffit d'ordonner les valeurs en une liste croissante et de choisir la valeur qui est au centre de cette liste

	// Dans le cas où le vecteur possède un nombre impair d'éléments (N), la médiane est le nombre en position int(N/2)+1
	if (v.size() % 2 == 1) {
		return v[static_cast<int>((v.size() / 2) + 1)];
	}
	else
	{
		// Dans le cas où le vecteur possède un nombre pair d'éléments (N), la médiane est la moyenne arithmétique des nombres en position N/2 et N/2+1
		const int k = static_cast<int>((v.size() / 2) - 1);
		return (v[k] + v[k + 1]) / 2.0;
	}
}




int main()
{
	try
	{
		// A mettre dans la 1ère ligne du "main" pour afficher les caractères accentués sous la console Windows
		std::locale::global(std::locale("fr-FR"));
		/*
		// ***************************************************************************************************************************************************************************************
		// 1. Lecture fichier de test : utilisation des techniques du cours
		// ***************************************************************************************************************************************************************************************
		string iname = "Abonnes.prn";
		//string iname = "Abonnes1.txt";
		
		// On ouvre le fichier présent sur le disque dur en spécifiant la gestion d'exception (page 357)
		ifstream ist{ iname };
		ist.exceptions(ist.exceptions() | ios_base::badbit);
		if (!ist)error("Impossible d'ouvrir le fichier ",iname);

		// On crée le vecteur qui va stocker les abonnés
		vector<Reading>abonnes;
		
		string str{};											// Cette variable "char" va stocker chaque enregistrement lu
		
		while (!ist.eof()) {									// Tant que la fin du fichier n'est pas atteinte, on continue à extraire de nouveaux enregistrements
			ist.clear();										// On "cleare" le flux SINON la lecture s'arrêt au bout de 38 enregistrements : INCOMPRIS à date
			getline(ist,str);									// On lit un nb de caractères max MAIS on s'arrête dès que l'on rencontre un NEWLINE
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
		
		
		
		// ***************************************************************************************************************************************************************************************
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
		
		*/

		// ***************************************************************************************************************************************************************************************
		// 2. Exercices pages 377 et 378
		// ***************************************************************************************************************************************************************************************
		
		// Exo 1 : somme des nombres inclus dans un fichier
		// ------------------------------------------------
		// Ouverture en lecture du fichier mydata2.txt
		string filename2 = "mydata2.txt";
		ifstream ist2{ filename2 };
		ist2.exceptions(ist2.exceptions() | ios_base::badbit);
		if (!ist2)error("Impossible d'ouvrir le fichier ", filename2);

		int somme = 0;
		int nb = 0;
		while (ist2 >>nb ) somme+=nb;	
		ist2.close();	// Fermeture du fichier

		cout <<"La somme des valeurs contenues dans le fichier est égale à "<<somme<< '\n';
		// --------------------------------------------------------------------------------

		// Exo 2 à 5 : variations autour des températures
		// ----------------------------------------------
		// Création d'un fichier pour écriture
		string filename3 = "raw_temps.txt";
		ofstream ost3{ filename3 };
		ost3.exceptions(ost3.exceptions() | ios_base::badbit);
		if (!ost3)error("Impossible d'ouvrir le fichier ", filename3);

		// Ecriture de 48 valeurs (2 par heure)
		uniform_int_distribution<>const uid3{ 0,30 };	// From TC++PL4, page 1186
		default_random_engine e;
		
		for (int hour=0;hour<24;++hour) {
			ost3 << hour << " " << uid3(e) << "\n";	
			ost3 << hour << " " << uid3(e) << "\n";

		}

		ost3.close();	// Fermeture du fichier

		// Ouverture en lecture du fichier raw_temps.txt
		ifstream ist3{ filename3 };
		ist3.exceptions(ist3.exceptions() | ios_base::badbit);
		if (!ist3)error("Impossible d'ouvrir le fichier ", filename3);

		// Moyenne et médiane des températures du fichier
		int nb3 = 0;
		int hour = 0;
		vector <int> temp_lue{};	// On crée un vecteur pour stocker les valeurs lues : plus simple ensuite de les utiliser avec les fonctions moyenne et médiane écrites dans un ancien exercice
		while (ist3 >> hour >>nb3) {
			temp_lue.push_back(nb3);
		}
		ist3.close();	// Fermeture du fichier

		cout << "La moyenne des "<<temp_lue.size()<< " valeurs contenues dans le fichier est de "<< moyenne_arithmetique(temp_lue) << " ; la médiane est égale à "<< mediane(temp_lue)<<"\n";

		// --------------------------------------------------------------------------------

		// Exo 8 : concaténation des 2 fichiers file4 et file5 dans file3
		// --------------------------------------------------------------
		
		string file1 = "Texte4.txt";
		string file2 = "Texte5.txt";
		string file3 = "Texte3.txt";
				
		// On ouvre les 2 fichiers source 
		ifstream istF1{ file1 };
		istF1.exceptions(istF1.exceptions() | ios_base::badbit);
		if (!istF1)error("Impossible d'ouvrir le fichier ", file1);

		ifstream istF2{ file2 };
		istF2.exceptions(istF2.exceptions() | ios_base::badbit);
		if (!istF2)error("Impossible d'ouvrir le fichier ", file2);

		// Création du fichier destination
		ofstream ostF3{ file3 };
		ostF3.exceptions(ostF3.exceptions() | ios_base::badbit);
		if (!ostF3)error("Impossible d'ouvrir le fichier ", file3);

		
		String str{};											// Cette variable "char" va stocker chaque enregistrement lu
		int nb_lignes = 0;

		while (!istF1.eof()) {									// Tant que la fin du fichier n'est pas atteinte, on continue à extraire de nouveaux enregistrements
			istF1.clear();										// On "cleare" le flux 
			getline(istF1,str);									// On lit un nb de caractères max MAIS on s'arrête dès que l'on rencontre un NEWLINE
			ostF3 << str<< "\n";
			++nb_lignes;
		}
		istF1.close();	// Fermeture du fichier

		while (!istF2.eof()) {									// Tant que la fin du fichier n'est pas atteinte, on continue à extraire de nouveaux enregistrements
			istF2.clear();										// On "cleare" le flux 
			getline(istF2,str);			// On lit un nb de caractères max MAIS on s'arrête dès que l'on rencontre un NEWLINE
			ostF3 << str << "\n";
			++nb_lignes;
		}
		istF2.close();	// Fermeture du fichier en lecture
		ostF3.close();	// Fermeture du fichier destination en écriture

		cout << "Les 2 fichiers Texte4.txt et Texte5.txt ont été concaténés dans Texte3.txt qui représente " << nb_lignes << " lignes\n";
		// --------------------------------------------------------------------------------

		// Exo 9 : concaténation des 2 fichiers file6 et file7 dans file8, tout en préservant un tri alphabétique
		// ------------------------------------------------------------------------------------------------------
		string file6 = "Texte6.txt";
		string file7 = "Texte7.txt";
		string file8 = "Texte8.txt";

		// On ouvre les 2 fichiers source 
		ifstream istF6{ file6 };
		istF6.exceptions(istF6.exceptions() | ios_base::badbit);
		if (!istF6)error("Impossible d'ouvrir le fichier ", file6);

		ifstream istF7{ file7 };
		istF7.exceptions(istF2.exceptions() | ios_base::badbit);
		if (!istF7)error("Impossible d'ouvrir le fichier ", file7);

		// Création du fichier destination
		ofstream ostF8{ file8 };
		ostF8.exceptions(ostF8.exceptions() | ios_base::badbit);
		if (!ostF8)error("Impossible d'ouvrir le fichier ", file8);

		string name = "";
		vector <string> sorted_names{};
		
		while (istF6 >> name)sorted_names.push_back(name);
		istF6.close();	// Fermeture du fichier

		while (istF7 >> name)sorted_names.push_back(name);
		istF7.close();	// Fermeture du fichier

		sort(sorted_names);	// Tri du vecteur
		for (auto x:sorted_names)
			ostF8 << x << "\n";

		ostF8.close();	// Fermeture du fichier destination en écriture
		
		cout << "Les 2 fichiers Texte6.txt et Texte7.txt ont été concaténés dans Texte8.txt (trié) qui représente " << sorted_names.size() << " lignes\n";
		
		// ***************************************************************************************************************************************************************************************
		// 3. Exercice 11 page 378 : somme de tous les entiers trouvés dans un fichier
		// ***************************************************************************************************************************************************************************************
		// On ouvre le fichier source 
		ifstream istF9{ "exo11.txt" };
		istF9.exceptions(istF6.exceptions() | ios_base::badbit);
		if (!istF9)error("Impossible d'ouvrir le fichier");

		string input{};
		int somme_int = 0;
		while (istF9 >> input)
		{
			if(isInteger_v2(input))somme_int += stoi(input);
		}

		cout << "La somme des entiers présents dans le fichier exo11.txt est égale à " << somme_int << '\n';

		istF9.close();	// Fermeture du fichier

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

