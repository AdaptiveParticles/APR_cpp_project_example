#include <iostream>


#include <functional>
#include <string>

#include "data_structures/APR/APR.hpp"
#include "data_structures/APR/particles/ParticleData.hpp"
#include "io/APRFile.hpp"

struct cmdLineOptions{
  std::string output = "output";
  std::string stats = "";
  std::string directory = "";
  std::string input = "";
  bool stats_file = false;
};

cmdLineOptions read_command_line_options(int argc, char **argv);

bool command_option_exists(char **begin, char **end, const std::string &option);

char* get_command_option(char **begin, char **end, const std::string &option);

int main(int argc, char **argv)  {


  std::cout << "Hello, World!" << std::endl;

  // INPUT PARSING

  cmdLineOptions options = read_command_line_options(argc, argv);

  // Filename
  std::string file_name = options.directory + options.input;

  // Read the apr file into the part cell structure
  APRTimer timer;

  timer.verbose_flag = true;

  // APR datastructure
  APR apr;

  timer.start_timer("read apr");
  //read file
  APRFile aprFile;
  aprFile.open(file_name,"READ");
  aprFile.read_apr(apr);

  ParticleData<uint16_t>parts;
  aprFile.read_particles(apr,"particles intensities", parts);

  aprFile.close();
  timer.stop_timer();

  return 0;
}


bool command_option_exists(char **begin, char **end, const std::string &option)
{
  return std::find(begin, end, option) != end;
}

char* get_command_option(char **begin, char **end, const std::string &option)
{
  char ** itr = std::find(begin, end, option);
  if (itr != end && ++itr != end)
  {
    return *itr;
  }
  return 0;
}

cmdLineOptions read_command_line_options(int argc, char **argv){

  cmdLineOptions result;

  if(argc == 1) {
    std::cerr << "Usage: \"Example_apr_iterate -i input_apr_file -d directory\"" << std::endl;

    exit(1);
  }

  if(command_option_exists(argv, argv + argc, "-i"))
  {
    result.input = std::string(get_command_option(argv, argv + argc, "-i"));
  } else {
    std::cout << "Input file required" << std::endl;
    exit(2);
  }

  if(command_option_exists(argv, argv + argc, "-d"))
  {
    result.directory = std::string(get_command_option(argv, argv + argc, "-d"));
  }

  if(command_option_exists(argv, argv + argc, "-o"))
  {
    result.output = std::string(get_command_option(argv, argv + argc, "-o"));
  }

  return result;

}
