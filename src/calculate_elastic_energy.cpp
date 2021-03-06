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

/* get squared residuals and subtract the displacement */
float get_sr(float num1, float num2, float offset)
{
    float sr = 0;
    
    sr = pow(num1 - offset - num2, 2);

    return sr;
}

double get_distance(std::vector<residue>& couple)
{
    float x1, y1, z1, x2, y2, z2;
    float dist_sq;

    x1 = couple[0].coordinate.x;
    x2 = couple[1].coordinate.x;
    y1 = couple[0].coordinate.y;
    y2 = couple[1].coordinate.y;
    z1 = couple[0].coordinate.z;
    z2 = couple[1].coordinate.z;
    
    dist_sq = pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2);

    return sqrt(dist_sq);
}

float calculate_ssr(std::vector<residue>& input1, 
                    std::vector<residue>& input2, coor& disp_vec)
{
    float result = 0;
    float dx2 = 0, dy2 = 0, dz2 = 0;
    
    /* from x0 to x7 */
    for (int i = 0; i < 8; i++) {
        dx2 += get_sr(input1[i].coordinate.x, input2[i].coordinate.x, disp_vec.x);
        dy2 += get_sr(input1[i].coordinate.y, input2[i].coordinate.y, disp_vec.y);
        dz2 += get_sr(input1[i].coordinate.z, input2[i].coordinate.z, disp_vec.z);
    }

    result = dx2 + dy2 + dz2;
    
    return result;
}

coor get_avg_displacement(std::vector<residue>& input, std::vector<residue>& mean_pos)
{
    float dx = 0, dy = 0, dz = 0;

    for (int i = 0; i < 8; i++) {
        dx += input[i].coordinate.x - mean_pos[i].coordinate.x;
        dy += input[i].coordinate.y - mean_pos[i].coordinate.y;
        dz += input[i].coordinate.z - mean_pos[i].coordinate.z;
    }

    dx /= 8;
    dy /= 8;
    dz /= 8;

    return coor(dx, dy, dz);
}

void get_dist_pairs(const std::vector<residue>& input_array, std::vector<std::vector<residue>>& pairs_vec)
{
    std::vector<residue> residue_array = input_array;
    std::vector<residue> one_pair;
    std::vector<residue>::iterator it = residue_array.begin();

    while (residue_array.size() > 1) {
        residue cur_res = residue_array.front();
        residue_array.erase(residue_array.begin());
        while (it != residue_array.end()) {
            one_pair.push_back(cur_res);
            one_pair.push_back(*it);
            pairs_vec.push_back(one_pair);
            one_pair.clear();
            it++;
        }
        it = residue_array.begin();
    }
}

float calculate_crystal_energy(std::vector<std::vector<residue>>& crystal_struct)
{
    float energy = 0;
    double dist0;

    for (size_t i = 0; i < crystal_struct.size(); i++) {
        dist0 = get_distance(crystal_struct[i]);
        energy += dist0 * dist0;
    }

    return energy;
}

float calculate_energy(std::vector<std::vector<residue>>& pairs_vec, std::vector<std::vector<residue>>& crystal_struct)
{
    /* number of pairs each time */
    int size = crystal_struct.size();
    float energy = 0;
    //di
    double disti;
    //d0
    double dist0;

    for (int i = 0; i < size; i++) {
        disti = get_distance(pairs_vec[i]);
        dist0 = get_distance(crystal_struct[i]);
        energy += pow(disti - dist0, 2) / dist0;
    }

    return energy;
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
            (res_buffer.end() - 1)->coordinate.update(x, y, z);
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
    std::ifstream crystal_file;
    std::ofstream energy_file;
    std::vector<residue> res_buffer;
    std::vector<residue> crystal_buffer;
    std::vector<std::vector<residue>> crystal_pairs;
    std::vector<std::vector<residue>> dist_pairs;
    float elastic_energy;
    std::string line, resname;
    float x, y, z;
    int ID;
    int frames = 1;
    coor displacement_offset;

    coor_file.open(argv[1]);
    crystal_file.open(argv[2]);
    energy_file.open(argv[3]);

    read_residues(crystal_file, crystal_buffer);
    get_dist_pairs(crystal_buffer, crystal_pairs);

    while (std::getline(coor_file, line)) {
        std::stringstream iss(line);
        /* read 8 residues */
        if (iss >> resname >> ID >> x >> y >> z) {
            res_buffer.push_back(residue(resname, ID));
            (res_buffer.end() - 1)->coordinate.update(x, y ,z);
        } else {
            /* sum up all frames of coordinates */
            if (res_buffer.size() > 0) {
                get_dist_pairs(res_buffer, dist_pairs);
                elastic_energy = calculate_energy(dist_pairs, crystal_pairs);
                energy_file << frames << " " << elastic_energy << std::endl;
                frames++;
                elastic_energy = 0;
                /* reset containers */
                res_buffer.clear();
                dist_pairs.clear();
            }
        }
    }

    std::cout << "crystal struct energy: " << calculate_crystal_energy(crystal_pairs) << std::endl;

    coor_file.close();
    crystal_file.close();
    energy_file.close();
}
