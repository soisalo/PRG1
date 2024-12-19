#include "datastructures.hh"
#include "customtypes.hh"

Datastructures::Datastructures()
{

}

Datastructures::~Datastructures()
{

}

unsigned int Datastructures::get_bite_count()
{

    return BiteMap.size();
}

void Datastructures::clear_all()
{

    BiteMap.clear();
    contourMap.clear();
}

std::vector<BiteID> Datastructures::all_bites()
{


    std::vector<BiteID> bite_ids;
  bite_ids.reserve(BiteMap.size());

    for (const auto& pair : BiteMap) {
      bite_ids.push_back(pair.first);
  }
    return bite_ids;
}

bool Datastructures::add_bite(BiteID id, const Name & name, Coord xy)
{


    if (BiteMap.find(id) != BiteMap.end()) {
      return false;
  }

    for (const auto& pair : BiteMap) {
      if (pair.second.coordinate == xy) {
            return false;
        }
  }
    BiteNode bite;
    bite.id = id;
    bite.name = name;
    bite.coordinate = xy;

    BiteMap.insert(std::pair<BiteID,BiteNode>(id,bite));
        return true;
    }


Name Datastructures::get_bite_name(BiteID id)
{

  auto it = BiteMap.find(id);
  if (it != BiteMap.end()) {
        return it->second.name;
    }
  else {
      return NO_NAME;
    }
}

Coord Datastructures::get_bite_coord(BiteID id)
{

    auto it = BiteMap.find(id);

  if (it != BiteMap.end()) {
        return it->second.coordinate;
    }
  else {
      return NO_COORD;
  }
}

std::vector<BiteID> Datastructures::get_bites_alphabetically()
{


    std::vector<BiteID> sorted_bite_ids;
    sorted_bite_ids.reserve(BiteMap.size());

    std::set<std::pair<Name, BiteID>> bites;

    for (const auto& pair : BiteMap) {
        bites.insert({pair.second.name, pair.first});
    }

    for (const auto& bite : bites) {
        sorted_bite_ids.push_back(bite.second);
    }
    return sorted_bite_ids;
}
std::vector<BiteID> Datastructures::get_bites_distance_increasing()
{


    std::vector<std::pair<BiteID, BiteNode>> bites(BiteMap.begin(),BiteMap.end());


    auto manhattan_distance = [](const Coord& coord) {
        return std::abs(coord.x) + std::abs(coord.y);
    };


    std::sort(bites.begin(), bites.end(), [&](const auto& a, const auto& b) {
        int dist_a = manhattan_distance(a.second.coordinate);
        int dist_b = manhattan_distance(b.second.coordinate);

        if (dist_a != dist_b) {
            return dist_a < dist_b;
        } else if (a.second.coordinate.y != b.second.coordinate.y) {
            return a.second.coordinate.y < b.second.coordinate.y;
        } else {
            return a.first < b.first;
        }
    });


    std::vector<BiteID> sorted_bite_ids;
    sorted_bite_ids.reserve(bites.size());
    for (const auto& bite : bites) {
        sorted_bite_ids.push_back(bite.first);
    }

    return sorted_bite_ids;
}

BiteID Datastructures::find_bite_with_coord(Coord xy)
{
    for (const auto& pair : BiteMap) {
        if (pair.second.coordinate == xy) {
            return pair.first;
        }
    }
    return NO_BITE;
}

bool Datastructures::change_bite_coord(BiteID id, Coord newcoord)
{


        auto biteIt = BiteMap.find(id);
        if (biteIt == BiteMap.end()) {
            return false;
        }


        for (const auto& bitePair : BiteMap) {
            if (bitePair.second.coordinate == newcoord) {
                return false;
            }
        }


        for (const auto& contourPair : contourMap) {
            const auto& contour = contourPair.second;
            if (std::find(contour.bites.begin(), contour.bites.end(), id) != contour.bites.end()) {

                if (std::find(contour.coordinates.begin(), contour.coordinates.end(), newcoord) == contour.coordinates.end()) {
                    return false;
                }
                break;
            }
        }


        biteIt->second.coordinate = newcoord;
        return true;
}

bool Datastructures::add_contour(ContourID id, const Name & name, ContourHeight height,std::vector<Coord> coords)
{


        if (contourMap.find(id) != contourMap.end()) {
            return false;
        }


        if (height > MAX_CONTOUR_HEIGHT || height < -MAX_CONTOUR_HEIGHT) {
            return false;
        }


        if (coords.empty()) {
            return false;
        }

        Contour newContour;
        newContour.id = id;
        newContour.height = height;
        newContour.name = name;
        newContour.coordinates = coords;
        newContour.parent = nullptr;

        contourMap.insert({id,newContour});
        return true;
}

std::vector<ContourID> Datastructures::all_contours()
{

        std::vector<ContourID> contour_ids;


        for (const auto& pair : contourMap) {
            contour_ids.push_back(pair.first);
        }
        return contour_ids;
}

Name Datastructures::get_contour_name(ContourID id)
{

        auto it = contourMap.find(id);

      if (it != contourMap.end()) {
            return it->second.name;
        }

      return NO_NAME;
}

std::vector<Coord> Datastructures::get_contour_coords(ContourID id)
{

    auto it = contourMap.find(id);

  if (it != contourMap.end()) {
        return it->second.coordinates;
    }

  return std::vector<Coord>{NO_COORD};
}

ContourHeight Datastructures::get_contour_height(ContourID id)
{

    auto it = contourMap.find(id);

  if (it != contourMap.end()) {
        return it->second.height;
    }

  return NO_CONTOUR_HEIGHT;
}

bool Datastructures::add_subcontour_to_contour(ContourID id,
                                               ContourID parentid)
{


        auto parentIt = contourMap.find(parentid);
        if (parentIt == contourMap.end()) {
            return false;
        }


        auto subContourIt = contourMap.find(id);
        if (subContourIt == contourMap.end()) {
            return false;
        }


        if (subContourIt->second.parent) {
            return false;
        }


        if (std::abs(subContourIt->second.height - parentIt->second.height) != 1) {
            return false;
        }


        if (std::abs(subContourIt->second.height) < std::abs(parentIt->second.height)) {
            return false;
        }


        parentIt->second.subcontours.push_back(std::make_shared<Contour>(subContourIt->second));
        subContourIt->second.parent = std::make_shared<Contour>(parentIt->second);

        return true;
}

bool Datastructures::add_bite_to_contour(BiteID biteid, ContourID contourid)
{


        auto biteIt = BiteMap.find(biteid);
        if (biteIt == BiteMap.end()) {
            return false;
        }


        auto contourIt = contourMap.find(contourid);
        if (contourIt == contourMap.end()) {
            return false;
        }


        if (biteIt->second.curCont != NO_CONTOUR) {
            return false;
        }


        Coord biteCoord = biteIt->second.coordinate;
        const auto& contourCoords = contourIt->second.coordinates;
        if (std::find(contourCoords.begin(), contourCoords.end(), biteCoord) == contourCoords.end()) {
            return false;
        }


        contourIt->second.bites.push_back(biteid);


        biteIt->second.curCont = contourid;

        return true;
}

std::vector<ContourID> Datastructures::get_bite_in_contours(BiteID id)
{

        std::vector<ContourID> contourIds;


        auto biteIt = BiteMap.find(id);
        if (biteIt == BiteMap.end()) {

            return {NO_CONTOUR};
        }


        for (const auto& contourPair : contourMap) {
            const auto& contour = contourPair.second;


            if (std::find(contour.bites.begin(), contour.bites.end(), id) != contour.bites.end()) {

                contourIds.push_back(contour.id);

                auto currentParent = contour.parent;


                while (currentParent) {
                    contourIds.push_back(currentParent->id);
                    currentParent = currentParent->parent;
                }

                break;
            }
        }

        return contourIds;
}

std::vector<ContourID>
Datastructures::all_subcontours_of_contour(ContourID )
{
  throw NotImplemented("all_subcontours_of_contour");
}

ContourID
Datastructures::get_closest_common_ancestor_of_contours(ContourID ,
                                                        ContourID )
{

  throw NotImplemented("get_closest_common_ancestor_of_contours");
}

bool Datastructures::remove_bite(BiteID )
{

  throw NotImplemented("remove_bite");
}

std::vector<BiteID> Datastructures::get_bites_closest_to(Coord )
{

  throw NotImplemented("get_bites_closest_to");
}
