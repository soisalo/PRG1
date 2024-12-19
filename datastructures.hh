

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH 

#include "customtypes.hh"
#include <unordered_map>
#include <utility>
#include <vector>


#include <iostream>
#include <algorithm>
#include <memory>

template <typename Type>
Type random_in_range(Type start, Type end);



class Datastructures
{
public:
  Datastructures();
  ~Datastructures();




  unsigned int get_bite_count();




  void clear_all();




  std::vector<BiteID> all_bites();




  bool add_bite(BiteID , const Name & , Coord );



  Name get_bite_name(BiteID );



  Coord get_bite_coord(BiteID );







  std::vector<BiteID> get_bites_alphabetically();




  std::vector<BiteID> get_bites_distance_increasing();




  BiteID find_bite_with_coord(Coord );




  bool change_bite_coord(BiteID , Coord );




  bool add_contour(ContourID , const Name & , ContourHeight ,
                   std::vector<Coord> );



  std::vector<ContourID> all_contours();



  Name get_contour_name(ContourID );




  std::vector<Coord> get_contour_coords(ContourID );




  ContourHeight get_contour_height(ContourID );




  bool add_subcontour_to_contour(ContourID , ContourID );




  bool add_bite_to_contour(BiteID biteid, ContourID );




  std::vector<ContourID> get_bite_in_contours(BiteID );





  std::vector<ContourID> all_subcontours_of_contour(ContourID );



  std::vector<BiteID> get_bites_closest_to(Coord );



  bool remove_bite(BiteID );



  ContourID get_closest_common_ancestor_of_contours(ContourID ,
                                                    ContourID );

private:

    struct BiteNode
    {
        BiteID id ;
        std::string name;
        Coord coordinate;
        ContourID curCont = NO_CONTOUR;
    };
    struct Contour {
            ContourID id;
            Name name;
            ContourHeight height;
            std::vector<Coord> coordinates;
            std::vector<std::shared_ptr<Contour>> subcontours;
            std::vector<BiteID> bites;
            std::shared_ptr<Contour> parent = nullptr;
        };

        std::unordered_map<BiteID,BiteNode> BiteMap;
        std::unordered_map<ContourID, Contour> contourMap;
};

#endif
