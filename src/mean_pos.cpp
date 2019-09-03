#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <complex>

class coor
{
    public:
        coor(float x_in, float y_in, float z_in)
        {
            x = x_in;
            y = y_in;
            z = z_in;
        }

        float x, y, z;
};

class residue
{
    public:
        residue(std::string resname, int resID)
        {
            resn = resname;
            resid = resID;
            coordinate.x = 0;
            coordinate.y = 0;
            coordinate.z = 0;
        }

    std::string resn;
    int resid;
    coor coordinate;
};

void sum_coor(residue& input_res, residue& output_res)
{
    output_res.coordinate.x += input_res.coordinate.x;
    output_res.coordinate.y += input_res.coordinate.y;
    output_res.coordinate.z += input_res.coordinate.z;
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "Usage: ./calculate_distance input output" << std::endl;

        return -1;
    }

    std::ifstream coor_file;
    std::ofstream mean_file;
    std::string line, resname;
    std::vector<residue> res_buffer;
    std::vector<residue> entrance_res;
    float x, y, z;
    int frames = 1;
    int ID;

    coor_file.open(argv[1]);
    mean_file.open(argv[2]);

    /* entrance coordinates */
    entrance_res.push_back(residue("SER", 66));
    entrance_res.push_back(residue("ALA", 63));
    entrance_res.push_back(residue("PRO", 59));
    entrance_res.push_back(residue("ALA", 29));
    entrance_res.push_back(residue("PHE", 28));
    entrance_res.push_back(residue("MET", 246));
    entrance_res.push_back(residue("LEU", 243));
    entrance_res.push_back(residue("ALA", 242));

    while (std::getline(coor_file, line)) {
        std::stringstream iss(line);
        /* read 8 residues */
        if (iss >> resname >> ID >> x >> y >> z) {
            res_buffer.push_back(residue(resname, ID));
            res_buffer.end().coodinate.x = x;
            res_buffer.end().coodinate.y = y;
            res_buffer.end().coodinate.z = z;
        } else {
            /* sum up all frames of coordinates */
            for (int i = 0; i < 8; i++) {
                sum_coor(res_buffer[i], entrance_res[i]);
            }
            frames++;
        }
    }

    coor_file.close();
    mean_file.close();
}
