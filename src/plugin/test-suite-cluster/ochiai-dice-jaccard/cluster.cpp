#include "cluster.hpp"
#include "Ochiai_Dice_JaccardTestSuiteClusterPlugin.h"

namespace Clustering{

  //
  // Dump a point
  //
  std::ostream& operator << (std::ostream& os, Point& p){
    
    BOOST_FOREACH(Point::value_type d, p){ os << d << " "; }    
    return os;
  }

  //
  // distance between two points
  //
  Distance distance(const Point& x, const Point& y)
  {
    Distance total = 0.0;
    Distance diff;
    
    Point::const_iterator cpx=x.begin(); 
    Point::const_iterator cpy=y.begin();
    Point::const_iterator cpx_end=x.end();
    for(;cpx!=cpx_end;++cpx,++cpy){
      diff = *cpx - *cpy;
      total += (diff * diff); 
    }
    return total;  // no need to take sqrt, which is monotonic
  }

  //
  // Dump collection of Points
  //
  std::ostream& operator << (std::ostream& os, Points& cps){
    BOOST_FOREACH(Points::value_type p, cps){
        /******************************
        os<<p <<std::endl;
        *******************************/
    }
    return os;
  }

  //
  // Dump a Set of points
  //
  std::ostream& operator << (std::ostream& os, SetPoints & sp){
    
    BOOST_FOREACH(SetPoints::value_type pid, sp){     
       os << "pid=" << pid << " " ;
    }
    return os;
  };

  //
  // Dump ClustersToPoints
  //
  std::ostream& operator << (std::ostream& os, ClustersToPoints & cp){
    ClusterId cid = 0;
    BOOST_FOREACH(ClustersToPoints::value_type set, cp){
      os << "clusteriddd["  << cid << "]" << "=(-" << set << "-)" << std::endl;
      cid++;
    }

    return os;
  }


  //
  // Dump ClustersToPoints
  //
  std::ostream& operator << (std::ostream& os, PointsToClusters & pc){
    PointId pid = 0;
    BOOST_FOREACH(PointsToClusters::value_type cid, pc){
      
      //std::cout << "pid[" << pid << "]=" << cid << std::endl;
      pid ++;
    }
    return os;
  }

  //
  // Init collection of points
  //  
  void PointsSpace::init_points( std::vector< std::vector<float> > floatVectors ) {

      for(int a=0 ; a<floatVectors[0].size();a++){
        Point p;
        for (std::vector<float>::iterator it = floatVectors[a].begin() ; it != floatVectors[a].end(); ++it)
            p.push_back( double(*it) );

        points__.push_back(p);
    }

  }  

  //
  // Zero centroids
  //
  void Clusters::zero_centroids() {
      
    BOOST_FOREACH(Centroids::value_type& centroid, centroids__){
      BOOST_FOREACH(Point::value_type& d, centroid){
	d = 0.0;
      }
    }
  }

  //
  // Compute Centroids
  //
  void Clusters::compute_centroids() {

    
    Dimensions i;
    ClusterId cid = 0;
    PointId num_points_in_cluster;
    // For each centroid
    BOOST_FOREACH(Centroids::value_type& centroid, centroids__){

      num_points_in_cluster = 0;

      // For earch PointId in this set
      BOOST_FOREACH(SetPoints::value_type pid, 
		    clusters_to_points__[cid]){
	
	Point p = ps__.getPoint(pid);
	//std::cout << "(" << p << ")";
	for (i=0; i<num_dimensions__; i++)
	  centroid[i] += p[i];	
	num_points_in_cluster++;
      }
      //
      // if no point in the clusters, this goes to inf (correct!)
      //
      for (i=0; i<num_dimensions__; i++)
	centroid[i] /= num_points_in_cluster;	
      cid++;
    }
  }

  //
  // Initial partition points among available clusters
  //
  void Clusters::initial_partition_points(){
    
    ClusterId cid;
    
    for (PointId pid = 0; pid < ps__.getNumPoints(); pid++){
      
      cid = pid % num_clusters__;

      points_to_clusters__[pid] = cid;
      clusters_to_points__[cid].insert(pid);
    }
    

    /**************************
    std::cout << "Points_to_clusters " << std::endl;
    std::cout << points_to_clusters__;
    std::cout << "Clusters_to_points " << std::endl;
    std::cout << clusters_to_points__;
    ***************************/


  };

  //
  // k-means
  //
  void Clusters::k_means(void){
    
    bool move;
    bool some_point_is_moving = true;
    unsigned int num_iterations = 0;
    PointId pid;
    ClusterId cid, to_cluster;
    Distance d, min;
    

    //
    // Initial partition of points
    //
    initial_partition_points();

    //
    // Until not converge
    //
    while (some_point_is_moving){

     /*******************************
      std::cout << std::endl << "*** Num Iterations "
        << num_iterations  << std::endl << std::endl ;;
     ********************************/

      some_point_is_moving = false;

      compute_centroids();
      //      std::cout << "Centroids" << std::endl << centroids__;      

      //
      // for each point
      //
      for (pid=0; pid<num_points__; pid++){
	
	// distance from current cluster
	min = 
	  Clustering::distance(centroids__[points_to_clusters__[pid]], 
			       ps__.getPoint(pid));

    /***********************
    std::cout << "pid[" << pid << "] in cluster="
		  << points_to_clusters__[pid] 
          << " with distance=" << min << std::endl;
    ************************/

	//
	// foreach centroid
	//
	cid = 0; 
	move = false;
	BOOST_FOREACH(Centroids::value_type c, centroids__){
	  

	  d = Clustering::distance(c, ps__.getPoint(pid));
	  if (d < min){
	    min = d;
	    move = true;
	    to_cluster = cid;

	    // remove from current cluster
	    clusters_to_points__[points_to_clusters__[pid]].erase(pid);

	    some_point_is_moving = true;
        /********************************************
        std::cout << "\tcluster=" << cid
              << " closer, dist=" << d << std::endl;
              ********************************************************/
	  }
	  cid++;
	}
	
	//
	// move towards a closer centroid 
	//
	if (move){
	  
	  // insert
	  points_to_clusters__[pid] = to_cluster;
	  clusters_to_points__[to_cluster].insert(pid);
      //std::cout << "\t\tmove to cluster=" << to_cluster << std::endl;
	}
      }      

      num_iterations++;
    } // end while (some_point_is_moving)

    //std::cout << std::endl << "Final clusters" << std::endl;
    //std::cout << clusters_to_points__;



  }

  
};
