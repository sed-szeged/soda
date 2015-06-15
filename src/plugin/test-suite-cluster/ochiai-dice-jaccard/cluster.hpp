/*
 *
 *Algorithm source code: http://codingplayground.blogspot.hu/2009/03/k-means-in-c.html
 *
 * */

#ifndef CLUSTER_H
#define CLUSTER_H 1

#include <set>
#include <vector>
#include <iostream>

#include <boost/unordered_map.hpp>
#include <boost/foreach.hpp>

#include "Ochiai_Dice_JaccardTestSuiteClusterPlugin.h"

namespace Clustering{

  typedef double Coord;            // a coordinate
  typedef double Distance;         // distance
  typedef unsigned int Dimensions; // how many dimensions
  typedef unsigned int PointId;    // the id of this point
  typedef unsigned int ClusterId;  // the id of this cluster

  typedef std::vector<Coord> Point;    // a point (a centroid)
  typedef std::vector<Point> Points;   // collection of points

  typedef std::set<PointId> SetPoints; // set of points
  
  // ClusterId -> (PointId, PointId, PointId, .... )
  typedef std::vector<SetPoints> ClustersToPoints;
  // PointId -> ClusterId
  typedef std::vector<ClusterId> PointsToClusters; 
  // coll of centroids
  typedef std::vector<Point> Centroids;

  //
  // Dump a point
  //
  std::ostream& operator << (std::ostream& os, Point& p);

  //
  // distance between two points
  //
  Distance distance(const Point & x, const Point & y);

  //
  // Dump collection of Points
  //
  std::ostream& operator << (std::ostream& os, Points& cps);

  //
  // Dump a Set of points
  //
  std::ostream& operator << (std::ostream& os, SetPoints & sp);
    
  //
  // Dump centroids
  //
  std::ostream& operator << (std::ostream& os, Centroids & cp);
  

  //
  // Dump ClustersToPoints
  //
  std::ostream& operator << (std::ostream& os, ClustersToPoints & cp);

  //
  // Dump ClustersToPoints
  //
  std::ostream& operator << (std::ostream& os, PointsToClusters & pc);


  //
  // This class stores all the points available in the model
  //
  class PointsSpace{

    //
    // Dump collection of points
    //
    friend std::ostream& operator << (std::ostream& os, PointsSpace & ps){

      PointId i = 0;
      BOOST_FOREACH(Points::value_type p, ps.points__){
    /***************
     os << "point["<<i++<<"]=" << p << std::endl;
     *******************/
      }

      return os;
    };

  public:

    PointsSpace(PointId num_points, Dimensions num_dimensions, std::vector< std::vector<float> > floatVectors )
      : num_points__(num_points), num_dimensions__(num_dimensions), floatVectors__(floatVectors)
    { init_points(floatVectors);};

    inline const PointId getNumPoints() const {return num_points__;}
    inline const PointId getNumDimensions() const {return num_dimensions__;}
    inline const Point& getPoint(PointId pid) const { return points__[pid];}
 
    //
    // Init collection of points
    //
    void init_points(std::vector< std::vector<float> > floatVectors__);

  private:  
    PointId num_points__;
    Dimensions num_dimensions__;
    Points points__;
    std::vector< std::vector<float> > floatVectors__;


  };

  // 
  //  This class represents a cluster
  // 
  class Clusters {

  private:
   
    ClusterId num_clusters__;    // number of clusters
    PointsSpace& ps__;           // the point space
    Dimensions num_dimensions__; // the dimensions of vectors
    PointId num_points__;        // total number of points
    ClustersToPoints clusters_to_points__;
    Centroids centroids__;

  public:
    PointsToClusters points_to_clusters__;


    //
    // Zero centroids
    //
    void zero_centroids();     

    //
    // Zero centroids
    //
    void compute_centroids();     
    
    //
    // Initial partition points among available clusters
    //
    void initial_partition_points();

  public:
   
    //
    // Dump ClustersToPoints
    //
    friend std::ostream& operator << (std::ostream& os, Clusters & cl){
      
      ClusterId cid = 0;
      BOOST_FOREACH(ClustersToPoints::value_type set, cl.clusters_to_points__){
    //os << "Cluster["<<cid<<"]=(";
	BOOST_FOREACH(SetPoints::value_type pid, set){
	  Point p = cl.ps__.getPoint(pid);
      //os << "(" << p << ")";
	}
    //os << ")" << std::endl;
	cid++;
      }
      return os;
    }
    

    Clusters(ClusterId num_clusters, PointsSpace & ps) 
      : num_clusters__(num_clusters), ps__(ps), 
	num_dimensions__(ps.getNumDimensions()),
	num_points__(ps.getNumPoints()),
	points_to_clusters__(num_points__, 0){

      ClusterId i = 0;
      Dimensions dim;
      for (; i < num_clusters; i++){
	Point point;   // each centroid is a point
	for (dim=0; dim<num_dimensions__; dim++) 
	  point.push_back(0.0);
	SetPoints set_of_points;

	// init centroids
	centroids__.push_back(point);  

	// init clusterId -> set of points
	clusters_to_points__.push_back(set_of_points);
	// init point <- cluster
      }
      /*
      std::cout << "Centroids" 
		<<std::endl<< centroids__;
      std::cout << "PointsToClusters" 
        <<std::endl<< points_to_clusters__;
      std::cout << "ClustersToPoints" 
        <<std::endl<< clusters_to_points__;*/

    };
    
    //
    // k-means
    //
    void k_means (void);
  };

};


#endif
