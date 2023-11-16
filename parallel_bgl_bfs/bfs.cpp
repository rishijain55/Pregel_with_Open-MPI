#include <boost/graph/use_mpi.hpp>
#include <boost/graph/distributed/mpi_process_group.hpp>
#include <boost/graph/distributed/adjacency_list.hpp>
#include <boost/graph/distributed/breadth_first_search.hpp>

typedef boost::adjacency_list<boost::vecS, boost::distributedS<boost::graph::distributed::mpi_process_group, boost::vecS>, boost::directedS> Graph;

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator mpi_comm;



    return 0;
}
