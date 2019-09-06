#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <complex>

/* potential energy */
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

float calculate_ssr(std::vector<residue>& input1, 
                    std::vector<residue>& input2)
{
    float result = 0;
    float dx2 = 0, dy2 = 0, dz2 = 0;
    
    /* from x0 to x7 */
    for (int i = 0; i < 8; i++) {
        dx2 += pow(input1[i].coordinate.x - input2[i].coordinate.x, 2);
        dy2 += pow(input1[i].coordinate.y - input2[i].coordinate.y, 2);
        dz2 += pow(input1[i].coordinate.z - input2[i].coordinate.z, 2);
    }

    result = dx2 + dy2 + dz2;
    
    return result;
}

void read_residues(std::ifstream& input_file, std::vector<residue>& res_buffer)
{
    std::string line, resname;
    float x, y, z;
    int ID;

    while (std::getline(input_file, line)) {
        std::stringstream iss(line);
        /* read 8 residues */
        if (iss >> resname >> ID >> x >> y >> z) {
            res_buffer.push_back(residue(resname, ID));
            (res_buffer.end() - 1)->coordinate.update(x, y ,z);
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc < 4) {
        std::cout << "Usage: ./calculate_elastic_energy coor mean output" 
        << std::endl;

        return -1;
    }

    std::ifstream coor_file;
    std::ifstream mean_file;
    std::ofstream energy_file;
    std::vector<residue> res_buffer;
    std::vector<residue> mean_buffer;
    std::vector<residue> energy_buffer;
    float elastic_energy;
    std::string line, resname;
    float x, y, z;
    int ID;
    int frames = 1;

    coor_file.open(argv[1]);
    mean_file.open(argv[2]);
    energy_file.open(argv[3]);

    read_residues(mean_file, mean_buffer);

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
                    elastic_energy += calculate_ssr(res_buffer, mean_buffer);
                }
            }
            energy_file << frames << ": " << elastic_energy << std::endl;
            frames++;
            elastic_energy = 0;
            res_buffer.clear();
        }
    }

    coor_file.close();
    mean_file.close();
    energy_file.close();
}
