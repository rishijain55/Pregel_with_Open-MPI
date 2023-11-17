#include <boost/graph/use_mpi.hpp>

// Communicate via MPI
#include <boost/graph/distributed/mpi_process_group.hpp>

// Breadth-first search algorithm
#include <boost/graph/breadth_first_search.hpp>

// Distributed adjacency list
#include <boost/graph/distributed/adjacency_list.hpp>

// METIS Input
#include <boost/graph/metis.hpp>

// Graphviz Output
#include <boost/graph/distributed/graphviz.hpp>

// For choose_min_reducer
#include <boost/graph/distributed/distributed_graph_utility.hpp>

// Standard Library includes
#include <fstream>
#include <string>
#include <set>

#ifdef BOOST_NO_EXCEPTIONS
void
boost::throw_exception(std::exception const& ex)
{
    std::cout << ex.what() << std::endl;
    abort();
}
#endif

using namespace boost;
using boost::graph::distributed::mpi_process_group;

/* An undirected graph with distance values stored on the vertices. */
typedef adjacency_list<vecS, distributedS<mpi_process_group, vecS>, directedS,
                       /*Vertex properties=*/property<vertex_distance_t, std::size_t> >
  Graph;



void get_graph(int numWorkers,int N, Graph &g){
    
    for(int workerId=0;workerId<numWorkers;workerId++){
        int start = (N/(numWorkers))*(workerId);
        int end = start + N/(numWorkers);
        if(workerId==numWorkers-1) end = N;
        for(int i=start;i<end;i++){
            std::set<int> adj;
            while(adj.size()!=10){
                int t = rand()%N;
                if(t==i) continue;
                adj.insert(t);
            }
            std::vector<int> targets(adj.begin(),adj.end());
            for(auto t:targets){
                add_edge(vertex(i, g), vertex(t, g), g);
            }
        }
    };
    return;
}

int main(int argc, char* argv[])
{
  boost::mpi::environment env(argc,argv);

    int numWorkers =  boost::mpi::communicator().size();
    int workerId = boost::mpi::communicator().rank();
    // Generate a simple graph
    int numVertices = 20;
    Graph g(numVertices);
    get_graph(numWorkers,numVertices,g);

  // Get vertex 0 in the graph
  graph_traits<Graph>::vertex_descriptor start = vertex(0, g);

  // Compute BFS levels from vertex 0
  property_map<Graph, vertex_distance_t>::type distance =
    get(vertex_distance, g);

  // Initialize distances to infinity and set reduction operation to 'min'
  BGL_FORALL_VERTICES(v, g, Graph) {
    put(distance, v, (std::numeric_limits<std::size_t>::max)());
  }
  distance.set_reduce(boost::graph::distributed::choose_min_reducer<std::size_t>());

  put(distance, start, 0);
  breadth_first_search
    (g, start,
     visitor(make_bfs_visitor(record_distances(distance, on_tree_edge()))));

  // Output the graph to terminal
  write_graphviz(std::cout, g, make_label_writer(get(vertex_distance, g)));

  return 0;
}