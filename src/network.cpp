#include "network.h"
#include "random.h"
#include <random>
#include <algorithm>
#include <functional>

//Values contient *new size* nouvelles valeurs générées aléatoirement (NORMALE, mean = 0, sd = 1)

void Network::resize(const size_t& new_size) {
	
	///On remplace les values par un tableau nouveau valeurs aléatoires = nouveaux noeuds 
	std::vector<double> new_values(new_size, RNG.normal());	
	values = new_values; 
	
} 

//Création d'un lien entre les noeuds a et b
bool Network::add_link(const size_t& a, const size_t& b) {
	
	//On cherche le noeud a dans le tableau des links : 
	auto search = links.find(a);
	
	if ((a != b) and ((a < values.size()) and (b < values.size()) ) ) {
			
			//Tableau de liens vide ou noeud a non trouvé : créer lien (a,b) 
			if ((links.empty()) or (search == links.end())) {
				links.insert(std::make_pair(a, b));
				links.insert(std::make_pair(b, a));
				
				return true ; 
			}
			
			//Tableau de liens non vide et noeud a trouvé : 
			else {	
				
				if (search != links.end()) {
					//Si a n'est pas déjà lié à b : 
					if ( (search->second) != b ) {
					
					links.insert(std::make_pair(a, b)); 
					links.insert(std::make_pair(b, a));  
					return true ; 
					}
					
					else { 
						return false ; //le lien existe déjà 
					}
				}
			}
	}
	
	else {
		return false ;
	}
} 

 //Création aléatoire de liens entre les noeuds
size_t Network::random_connect(double const& mean_deg) {
	
	
	int compteur_lien = 0 ; 
	int taille_tab = 0 ; 
	
	std::vector<int> noeuds_random ;
	
	std::default_random_engine generateur;
	
	for (int i(0) ; i < values.size() ; ++i) {
		
		taille_tab = RNG.poisson(mean_deg);
		noeuds_random.resize(taille_tab);
		RNG.uniform_int(noeuds_random, 0, values.size() - 1);
		
		for (int a(0) ; a < noeuds_random.size() ; ++ a)
		 {
			if (add_link(i, noeuds_random[a])) {
				
				++compteur_lien ; 
			}
		}
	}
	

	
	return compteur_lien ; 


} 

//Nouvelles valeurs pour les noeuds  
size_t Network::set_values(std::vector<double>& potential_nodes) {
	
	unsigned int nodes_reset;
	unsigned int n = potential_nodes.size();
	unsigned int s = values.size(); 
	
	if (n < s) {
		for (int i(0) ; i < n ; ++i) {
			values[i] = potential_nodes[i] ; 
		}
		
		return n ; 
	}
	
	if (n > s) {
		
		potential_nodes.erase(potential_nodes.begin()+s, potential_nodes.end()) ;
		values = potential_nodes ;
		
		return s ; 
	}
} 

//Taille du tableau des values
size_t Network::size() const {
	
	return values.size() ;
}

//Nombre de connections pour un noeud
size_t Network::degree(const size_t &n) const {
	
	return links.count(n); //on ne doit pas compter (a,b) ET (b,a) pour dénombrer le nombre de liens
}

//Valeur associée au noeud n
double Network::value(const size_t &n) const {
	
	return values[n]; 
	
} 

//Classement décroissant des valeurs 
std::vector<double> Network::sorted_values() const {
	
	std::vector<double> values_to_sort = values ; 
	std::sort(values_to_sort.begin(), values_to_sort.end(), std::greater<double>()); 
	return values_to_sort; 
	
} 

//Voisins d'un noeud
std::vector<size_t> Network::neighbors(const size_t& n) const {
	
	std::vector<size_t> neighbors; 
	
	auto range = links.equal_range(n);
 
    for (auto i = range.first; i != range.second; ++i)
    {
        neighbors.push_back(i->second) ; 
    }
	
	return neighbors ; 
} 

