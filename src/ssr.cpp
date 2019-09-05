#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <complex>

/* sum of squared residuals */

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

std::vector<residue>& calculate_ssr(std::vector<residue>& input1, 
                                    std::vector<residue>& input2)
{
    
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
        std::cout << "Usage: ./ssr coor1 coor2 output" << std::endl;

        return -1;
    }

    std::ifstream coor_file1;
    std::ifstream coor_file2;
    std::ofstream ssr_file;
    std::string line, resname;
    std::vector<residue> res_buffer1;
    std::vector<residue> res_buffer2;
    float x, y, z;
    int ID;

    coor_file1.open(argv[1]);
    coor_file2.open(argv[2]);
    ssr_file.open(argv[3]);

    for (int i = 0; i < 8; i++) {
        avg_coor(res_buffer1[i], frames);
        mean_file << res_buffer1[i].resn << " " << res_buffer1[i].resid 
        << " " << res_buffer1[i].coordinate.x << " " 
        << res_buffer1[i].coordinate.y << " " << res_buffer1[i].coordinate.z 
        << std::endl;
    }

    coor_file1.close();
    coor_file2.close();
    ssr_file.close();
}
