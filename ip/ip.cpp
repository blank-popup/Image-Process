#include "pch.h"
#include <ShellAPI.h>
#include "ip.h"

int main(int argc, char const* argv[]) {
    boost::program_options::options_description description("Options");
    description.add_options()
        ("command,c", boost::program_options::value<std::string>(), "command to execute")
        ("files,f", boost::program_options::value<std::vector<std::string>>(), "Files for command")
        ("help,h", "display help message");

    boost::program_options::positional_options_description positionals;
    positionals.add("command", 1).add("files", -1);

    try {
        boost::program_options::variables_map vm;
        boost::program_options::command_line_parser command_line = boost::program_options::command_line_parser(argc, argv);
        boost::program_options::parsed_options parsed = command_line.options(description).positional(positionals).allow_unregistered().run();
        boost::program_options::store(parsed, vm);
        boost::program_options::notify(vm);

        if (vm.count("command")) {
            std::string command = vm["command"].as<std::string>();
            std::cout << "COMAND: " << command << "\n";

            // greeting
            if (command.compare("greeting") == 0) {
                ipl_say_greeting("HELLO WORLD, ITS-I !!!");
            }
            // blend -f d:\\WorkSpace\\ip\\lena_std.tif d:\\WorkSpace\\ip\\white512.png
            else if (command.compare("blend") == 0) {
                std::vector<std::string> files = vm["files"].as<std::vector<std::string>>();
                if (files.size() == 2) {
                    for (double alpha = 0.0; alpha <= 1.0; alpha += 0.2)
                    {
                        cv::Mat mat;
                        ipl_blend(mat, files[0], files[1], alpha);
                        ipl_display_cv_window("Test Image", mat);
                    }
                }
                else {
                    std::cout << "CANNOT execute: " << vm["command"].as<std::string>() << " because the count of files is not 2" << "\n";
                }
            }
            // gui
            else if (command.compare("gui") == 0) {
                ShellExecute(NULL, L"open", L"ipw.exe", NULL, NULL, SW_SHOW);
            }
            else {
                std::cout << "CANNOT execute: " << command << "\n";
            }
        }
        else {
            std::cout << description << "\n";
        }
    }
    catch (const boost::program_options::error& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << description << "\n";
    }

    return 0;
}
