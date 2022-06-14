//////////////////////////////////////////////////////
///
/// Bevan Cheeseman 2021
///

const char* usage = R"(
Form the APR form image: Takes an uint16_t input tiff image and forms the APR and saves it as hdf5. The hdf5 output of this program
can be used with the other apr examples, and also viewed with HDFView.

Usage:

(minimal with auto-parameters)

Example_get_apr -i input_image_tiff -d input_directory [-o name_of_output] -auto_parameters

Note: auto_parameters sets some parameters by a heuristic, which does not always work very well

Additional settings (High Level):

-grad_th    ignore areas in the image where the gradient magnitude is lower than this value

Advanced (Direct) Settings:

-lambda lambda_value (directly set the value of the gradient smoothing parameter lambda (reasonable range 0.1-10, default: 3)
-rel_error rel_error_value (Reasonable ranges are from .08-.15), Default: 0.1

Please explore the python wrappers and the interactive APR mode to understand how to set parameters for your given example.

)";

#include <algorithm>
#include <iostream>
#include <functional>
#include <string>

#include "APR/ConfigAPR.h"
#include "APR/io/APRFile.hpp"
#include "APR/algorithm/APRConverter.hpp"

struct cmdLineOptions{

    std::string output_dir = "";
    std::string output = "output";
    std::string directory = "";
    std::string input = "";

    bool auto_parameters = false;

    float lambda = 3;
    float grad_th = 2;
};

bool command_option_exists(char **begin, char **end, const std::string &option);

char* get_command_option(char **begin, char **end, const std::string &option);

cmdLineOptions read_command_line_options(int argc, char **argv);


int runAPR(cmdLineOptions options) {
    //the apr datastructure
    APR apr;

    APRConverter<float> aprConverter;

    //read in the command line options into the parameters file

    aprConverter.par.lambda = options.lambda;
    aprConverter.par.grad_th = options.grad_th;
    aprConverter.par.auto_parameters = options.auto_parameters;

    //where things are
    aprConverter.par.input_image_name = options.input;
    aprConverter.par.input_dir = options.directory;
    aprConverter.par.name = options.output;
    aprConverter.par.output_dir = options.output_dir;
    aprConverter.total_timer.verbose_flag = true;

    APRTimer timer;

    timer.verbose_flag = true;

    timer.start_timer("Load tiff");

    PixelData<uint16_t> input_img = TiffUtils::getMesh<uint16_t>(options.directory + options.input);

    timer.stop_timer();

    //Gets the APR
    if(aprConverter.get_apr(apr, input_img)){

        ParticleData<uint16_t> particle_intensities;
        particle_intensities.sample_image(apr, input_img); // sample your particles from your image

        //output
        std::string save_loc = options.output_dir;
        std::string file_name = options.output;

        std::cout << std::endl;
        float original_pixel_image_size = (2.0f* apr.org_dims(0)* apr.org_dims(1)* apr.org_dims(2))/1000000.0f;
        std::cout << "Original image size: " << original_pixel_image_size << " MB" << std::endl;

        timer.start_timer("writing output");

        std::cout << "Writing the APR to hdf5..." << std::endl;

        //write the APR to hdf5 file
        APRFile aprFile;

        aprFile.open(save_loc + file_name + ".apr");

        aprFile.write_apr(apr);
        aprFile.write_particles("particles", particle_intensities);

        float apr_file_size = aprFile.current_file_size_MB();

        timer.stop_timer();

        float computational_ratio = (1.0f* apr.org_dims(0)* apr.org_dims(1)* apr.org_dims(2))/(1.0f*apr.total_number_particles());

        std::cout << std::endl;
        std::cout << "Computational Ratio (#Pixels/#Particles): " << computational_ratio << std::endl;
        std::cout << "Lossy Compression Ratio: " << original_pixel_image_size/apr_file_size << std::endl;
        std::cout << std::endl;

    } else {
        std::cout << "Oops, something went wrong. APR not computed :(." << std::endl;
    }
    return 0;
}


int main(int argc, char **argv) {

    //input parsing
    cmdLineOptions options = read_command_line_options(argc, argv);

    return runAPR(options);
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
        std::cerr << argv[0] << std::endl;
        std::cerr << "APR version " << ConfigAPR::APR_VERSION << std::endl;
        std::cerr << "Short usage: \"" << argv[0] << " -i inputfile [-d directory] [-o outputfile]\"" << std::endl;

        std::cerr << usage << std::endl;
        exit(1);
    }

    if(command_option_exists(argv, argv + argc, "-i"))
    {
        result.input = std::string(get_command_option(argv, argv + argc, "-i"));
    } else {
        std::cout << "Input file required" << std::endl;
        exit(2);
    }

    if(command_option_exists(argv, argv + argc, "-o"))
    {
        result.output = std::string(get_command_option(argv, argv + argc, "-o"));
    }

    if(command_option_exists(argv, argv + argc, "-d"))
    {
        result.directory = std::string(get_command_option(argv, argv + argc, "-d"));
    }

    if(command_option_exists(argv, argv + argc, "-od"))
    {
        result.output_dir = std::string(get_command_option(argv, argv + argc, "-od"));
    } else {
        result.output_dir = result.directory;
    }

    if(command_option_exists(argv, argv + argc, "-lambda"))
    {
        result.lambda = std::stof(std::string(get_command_option(argv, argv + argc, "-lambda")));
    }

    if(command_option_exists(argv, argv + argc, "-grad_th"))
    {
        result.grad_th = std::stof(std::string(get_command_option(argv, argv + argc, "-grad_th")));
    }

    if(command_option_exists(argv, argv + argc, "-auto_parameters"))
    {
        result.auto_parameters = true;
    }

    return result;
}
