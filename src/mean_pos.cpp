#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <complex>

class coor
{
    public:
        coor(void)
        {
            x = 0;
            y = 0;
            z = 0;
        }

        coor(float x_in, float y_in, float z_in)
        {
            x = x_in;
            y = y_in;
            z = z_in;
        }

        void update(float new_x, float new_y, float new_z)
        {
            x = new_x;
            y = new_y;
            z = new_z;
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

void avg_coor(residue& input_res, int N)
{
    input_res.coordinate.x /= N;
    input_res.coordinate.y /= N;
    input_res.coordinate.z /= N;
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
            (res_buffer.end() - 1)->coordinate.update(x, y ,z);
        } else {
            /* sum up all frames of coordinates */
            if (res_buffer.size() > 0) {
                for (int i = 0; i < 8; i++) {
                    sum_coor(res_buffer[i], entrance_res[i]);
                }
            }
            
            res_buffer.clear();
            frames++;
        }
    }

    mean_file << "avg coordinates: \n";

    for (int i = 0; i < 8; i++) {
        avg_coor(entrance_res[i], frames);
        mean_file << entrance_res[i].resn << " " << entrance_res[i].resid 
        << " " << entrance_res[i].coordinate.x << " " 
        << entrance_res[i].coordinate.y << " " << entrance_res[i].coordinate.z 
        << std::endl;
    }


    coor_file.close();
    mean_file.close();
}
