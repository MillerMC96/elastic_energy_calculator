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
        std::cout << "Usage: ./ssr coor1 coor2 output" << std::endl;

        return -1;
    }

    std::ifstream coor_file1;
    std::ifstream coor_file2;
    std::ofstream ssr_file;
    std::vector<residue> res_buffer1;
    std::vector<residue> res_buffer2;
    float ssr;

    coor_file1.open(argv[1]);
    coor_file2.open(argv[2]);
    ssr_file.open(argv[3]);

    read_residues(coor_file1, res_buffer1);
    read_residues(coor_file2, res_buffer2);
    ssr = calculate_ssr(res_buffer1, res_buffer2);

    ssr_file << "Sum of squared residuals: " << ssr << std::endl;

    coor_file1.close();
    coor_file2.close();
    ssr_file.close();
}
