#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <string>
#include <bitset>
#include <algorithm>
#include <numeric>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
#include <sstream>

using namespace std;

int random(int t1, int t2)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> dis(t1, t2);
    return dis(gen);
}

double randomfloat(int t1, int t2)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(t1, t2);
    return dis(gen);
}

string intToBinary(int number, int size = 8)
{
    bitset<8> bits(number);
    return bits.to_string().substr(8 - size, size);
}
string intToBinary16(int number, int size = 16)
{
    bitset<16> bits(number);
    return bits.to_string().substr(16 - size, size);
}

int binaryToInt(string binary)
{
    // if(binary.length() == 16)
    //     cout << binary << '\n';
    std::bitset<16> bits(binary);
    return bits.to_ulong();
}

int getInterval(int X) { return binaryToInt(intToBinary16(X).substr(8, 8)); }
int getValue(int X) { return binaryToInt(intToBinary16(X).substr(0, 8)); }

class Crone
{
private:
    int N, M, a, b;
    vector<int> T = {};

public:
    Crone(int N, int M, int a, int b)
    {
        this->N = N;
        this->M = M - 1;
        this->a = a;
        this->b = b;
        for (int i = 0; i <= this->M; ++i)
            T.push_back(random(a, b));
    }

    Crone(int N, int M, vector<int> T)
    {
        this->N = N;
        this->M = M;
        this->T = T;
        this->a = 0;
        this->b = 0;
    }

    std::string Print(vector<vector<int>> &Darvin)
    {
        cout << endl;
        std::string data = "";
        for (int i = 0; i < Darvin.size(); ++i)
        {
            for (int j = 0; j < Darvin[i].size(); ++j)
            {
                cout << Darvin[i][j] << ' ';
                // data += to_string(Darvin[i][j]) + ' ';
            }
            // data += '\n';
            cout << endl;
        }
        return data;
    }

    std::string Print(vector<int> &OBJECTect)
    {
        std::string data = "";
        for (int i = 0; i < OBJECTect.size(); ++i)
        {
            cout << OBJECTect[i] << ' ';
            data += to_string(OBJECTect[i]);
            if (i != OBJECTect.size() - 1)
                data += ", ";
        }
        cout << endl;
        return data;
    }

    std::string getT()
    {
        return Print(T);
    }

    vector<int> cum_finder(vector<vector<int>> columns)
    {
        int sum = 0;
        vector<int> summ;
        if (N != columns.size())
            cout << "ARE YOU TALKING TO ME???\n";
        for (int i = 0; i < columns.size(); ++i)
        {
            sum = accumulate(columns[i].begin(), columns[i].end(), 0);
            summ.push_back(sum);
        }
        return summ;
    }

    vector<int> MaxMin(vector<int> object)
    {

        int max = -1;
        int min = 10000000;
        int pos_max = 0, pos_min = 0;
        for (int j = 0; j < object.size(); ++j)
        {
            if (object[j] != -1)
            {
                if (max < object[j])
                {
                    max = object[j];
                    pos_max = j;
                }
                if (min > object[j])
                {
                    min = object[j];
                    pos_min = j;
                }
            }
        }
        return vector<int>{max, pos_max, min, pos_min};
    }

    std::vector<std::vector<int>> criticalPath(int mod = 1)
    {
        vector<int> T0 = T;
        switch (mod)
        {
        case 0:
            break;

        case 1:
        {
            // от меньшего к большему
            sort(T0.begin(), T0.end(), less<int>());
            break;
        }

        case 2:
        {
            // от большего к меньшему
            sort(T0.begin(), T0.end(), greater<int>());
            break;
        }

        default:
            break;
        }

        std::vector<int> Nagr(N, 0);
        std::vector<std::vector<int>> Nabor(N);

        for (int i = 0; i <= M; ++i)
        {
            int minim = 10000000;
            int pos = 0;
            for (int k = 0; k < N; ++k)
            {
                if (Nagr[k] < minim)
                {
                    minim = Nagr[k];
                    pos = k;
                }
            }
            Nagr[pos] += T[i];
            Nabor[pos].push_back(T0[i]);
        }

        // for (vector<int> a : Nabor) {
        //     for (int i : a)
        //         cout << i << " ";
        //     cout << '\n';
        // }

        int maxim = *std::max_element(Nagr.begin(), Nagr.end());
        return Nabor;
    }

    vector<vector<int>> croneProcess(vector<vector<int>> fromGoldberg = {})
    {
        std::vector<std::vector<int>> columnss(N);
        if (fromGoldberg.empty())
            for (int i = 0; i <= M; ++i)
                columnss[(int)random(0, N - 1)].push_back(T[i]);
        else
            columnss = fromGoldberg;

        // Print(columnss);
        bool crone_check = true;
        vector<int> summ = cum_finder(columnss);
        /*cout << "summ is : " << endl;
        Print(summ);*/

        while (crone_check)
        {
            vector<int> maxmin_info = MaxMin(summ);
            // cout << "pos_max: " << maxmin_info[1] << ", pos_min: " << maxmin_info[3] << endl;
            int delta = maxmin_info[0] - maxmin_info[2];
            for (int i = 0; i < columnss[maxmin_info[1]].size(); ++i)
            {
                crone_check = false;
                if (columnss[maxmin_info[1]][i] < delta)
                {
                    int tmp = columnss[maxmin_info[1]][i];
                    auto iter = columnss[maxmin_info[1]].cbegin();
                    columnss[maxmin_info[1]].erase(iter + i);
                    columnss[maxmin_info[3]].push_back(tmp);
                    crone_check = true;
                    break;
                }
            }
            summ = cum_finder(columnss);
            // Print(summ);
        }
        while (true)
        {
            summ = cum_finder(columnss);
            // Print(summ);
            vector<int> maxmin_info = MaxMin(summ);
            int delta = maxmin_info[0] - maxmin_info[2];
            if (delta <= 1)
            {
                break;
            }
            bool changepoint = true;
            for (int i = 0; i < columnss[maxmin_info[1]].size(); ++i)
            {
                for (int j = 0; j < columnss[maxmin_info[3]].size(); ++j)
                {
                    int delta_tmp = columnss[maxmin_info[1]][i] - columnss[maxmin_info[3]][j];
                    if ((delta_tmp < delta) && (delta_tmp > 0))
                    {
                        int tmp = columnss[maxmin_info[1]][i];
                        columnss[maxmin_info[1]][i] = columnss[maxmin_info[3]][j];
                        columnss[maxmin_info[3]][j] = tmp;
                        changepoint = false;
                        break;
                    }
                }
                if (!changepoint)
                {
                    break;
                }
            }
            if (changepoint)
            {
                cout << "did work\n";
                break;
            }
        }
        // if (!fromGoldberg.empty()) {
        //     Print(columnss);
        // }

        Print(columnss);
        // for (int i : cum_finder(columnss)) {
        //     cout << i << ' ';
        // }
        // cout << '\n';

        return columnss;
    }

    vector<vector<int>> croneProcessModified(vector<vector<int>> fromGoldberg = {})
    {
        std::vector<std::vector<int>> columnss(N);
        if (fromGoldberg.empty())
            for (int i = 0; i <= M; ++i)
                columnss[(int)random(0, N - 1)].push_back(T[i]);
        else
            columnss = fromGoldberg;

        // Print(columnss);
        bool crone_check = true;
        vector<int> summ = cum_finder(columnss);
        /*cout << "summ is : " << endl;
        Print(summ);*/

        while (crone_check)
        {
            vector<int> maxmin_info = MaxMin(summ);
            // cout << "pos_max: " << maxmin_info[1] << ", pos_min: " << maxmin_info[3] << endl;
            int delta = maxmin_info[0] - maxmin_info[2];
            for (int i = 0; i < columnss[maxmin_info[1]].size(); ++i)
            {
                crone_check = false;
                if (columnss[maxmin_info[1]][i] < delta)
                {
                    int tmp = columnss[maxmin_info[1]][i];
                    auto iter = columnss[maxmin_info[1]].cbegin();
                    columnss[maxmin_info[1]].erase(iter + i);
                    columnss[maxmin_info[3]].push_back(tmp);
                    crone_check = true;
                    break;
                }
            }
            summ = cum_finder(columnss);
            // Print(summ);
        }

        int borderland = 0;
        summ = cum_finder(columnss);
        while (true)
        {
            // Print(summ);
            vector<int> maxmin_info = MaxMin(summ);

            if (maxmin_info[1] == maxmin_info[3])
                break;
            int delta = maxmin_info[0] - maxmin_info[2];
            // if (delta <= 1) {
            //     cout << "holy moly fuck\n";
            //     break;
            // }
            bool changepoint = true;
            for (int i = 0; i < columnss[maxmin_info[1]].size(); ++i)
            {
                for (int j = 0; j < columnss[maxmin_info[3]].size(); ++j)
                {
                    int delta_tmp = columnss[maxmin_info[1]][i] - columnss[maxmin_info[3]][j];
                    if ((delta_tmp < delta) && (delta_tmp > 0))
                    {
                        int tmp = columnss[maxmin_info[1]][i];
                        columnss[maxmin_info[1]][i] = columnss[maxmin_info[3]][j];
                        columnss[maxmin_info[3]][j] = tmp;
                        changepoint = false;
                        break;
                    }
                }
                if (!changepoint)
                {
                    break;
                }
            }
            if (changepoint)
            {
                Print(summ);
                summ[maxmin_info[3]] = -1;
                cout << "did work\n";
                Print(summ);
                continue;
            }
            summ = cum_finder(columnss);
        }
        // if (!fromGoldberg.empty()) {
        //     Print(columnss);
        // }

        Print(columnss);
        // for (int i : cum_finder(columnss)) {
        //     cout << i << ' ';
        // }
        // cout << '\n';

        return columnss;
    }

    int MaxCumFinder(vector<int> summ)
    {
        return (int)*max_element(summ.begin(), summ.end());
    }

    vector<vector<int>> Pashkeev(int mod = 0)
    {
        vector<int> T0 = T;
        switch (mod)
        {
        case 0:
            break;

        case 1:
        {
            // от меньшего к большему
            sort(T0.begin(), T0.end(), less<int>());
            break;
        }

        case 2:
        {
            // от большего к меньшему
            sort(T0.begin(), T0.end(), greater<int>());
            break;
        }

        default:
            break;
        }

        vector<vector<int>> Container(N);
        int i = 0;
        while (i <= M)
        {

            int leftBorder = 0, rightBorder = 0;
            for (int j = 0; j < N; ++j)
            {
                Container[j].push_back(T0[i]);
                ++i;
                if (i > M)
                    break;
            }
            vector<int> summ_data = cum_finder(Container);
            leftBorder = summ_data[0];
            rightBorder = summ_data[summ_data.size() - 1];

            if (leftBorder > rightBorder)
                std::reverse(Container.begin(), Container.end());
        }
        return Container;
    }
};

void writeData(std::string filename, vector<int> Ns, vector<int> Ms, vector<int> Interval, int MAX)
{
    string Tname = "T_" + filename;
    cout << Tname << '\n';

    int a = Interval[0], b = Interval[1];
    std::ofstream W("Data/" + filename);
    std::ofstream T("T/" + Tname);
    if (W.is_open() && T.is_open())
    {
        T << ",T, " << "N, " << "M, " << 'i' << '\n';
        for (int N = Ns[0]; N <= Ns[1]; ++N)
        {
            for (int &M : Ms)
            {
                for (int k = 0; k <= MAX; ++k)
                {
                    cout << "OH SHIT\n";
                    Crone Crn(N, M, a, b);
                    T << '[' << Crn.getT() << "], " << N << ", " << M << ", " << k << '\n';
                    vector<vector<int>> Res1 = Crn.criticalPath(0);
                    vector<int> CRes1 = Crn.cum_finder(Res1);
                    int Max = *max_element(CRes1.begin(), CRes1.end());
                    W << "Critical_Path : " << Crn.Print(CRes1) << ": " << Max << " : " << N << " : " << M << '\n';

                    Res1 = Crn.croneProcess(Res1);
                    CRes1 = Crn.cum_finder(Res1);
                    Max = *max_element(CRes1.begin(), CRes1.end());
                    W << "Critical_Path-Crone : " << Crn.Print(CRes1) << ": " << Max << " : " << N << " : " << M << '\n';

                    Res1 = Crn.criticalPath(1);
                    CRes1 = Crn.cum_finder(Res1);
                    Max = *max_element(CRes1.begin(), CRes1.end());
                    W << "Critical_Path_Less : " << Crn.Print(CRes1) << ": " << Max << " : " << N << " : " << M << '\n';

                    Res1 = Crn.croneProcess(Res1);
                    CRes1 = Crn.cum_finder(Res1);
                    Max = *max_element(CRes1.begin(), CRes1.end());
                    W << "Critical_Path_Less-Crone : " << Crn.Print(CRes1) << ": " << Max << " : " << N << " : " << M << '\n';

                    Res1 = Crn.criticalPath(2);
                    CRes1 = Crn.cum_finder(Res1);
                    Max = *max_element(CRes1.begin(), CRes1.end());
                    W << "Critical_Path_Greater : " << Crn.Print(CRes1) << ": " << Max << " : " << N << " : " << M << '\n';

                    Res1 = Crn.croneProcess(Res1);
                    CRes1 = Crn.cum_finder(Res1);
                    Max = *max_element(CRes1.begin(), CRes1.end());
                    W << "Critical_Path_Greater-Crone : " << Crn.Print(CRes1) << ": " << Max << " : " << N << " : " << M << '\n';
                    //================================================================================================================================
                    vector<vector<int>> Res2 = Crn.croneProcess();
                    vector<int> CRes2 = Crn.cum_finder(Res2);
                    Max = *max_element(CRes2.begin(), CRes2.end());
                    W << "Crone : " << Crn.Print(CRes2) << ": " << Max << " : " << N << " : " << M << '\n';

                    Res2 = Crn.croneProcessModified();
                    CRes2 = Crn.cum_finder(Res2);
                    Max = *max_element(CRes2.begin(), CRes2.end());
                    W << "Crone_Mod : " << Crn.Print(CRes2) << ": " << Max << " : " << N << " : " << M << '\n';

                    Res2 = Crn.Pashkeev();
                    CRes2 = Crn.cum_finder(Res2);
                    Max = *max_element(CRes2.begin(), CRes2.end());
                    W << "Pashkeev : " << Crn.Print(CRes2) << ": " << Max << " : " << N << " : " << M << '\n';

                    Res2 = Crn.croneProcess(Res2);
                    CRes2 = Crn.cum_finder(Res2);
                    Max = *max_element(CRes2.begin(), CRes2.end());
                    W << "Pashkeev-Crone : " << Crn.Print(CRes2) << ": " << Max << " : " << N << " : " << M << '\n';

                    Res2 = Crn.Pashkeev(1);
                    CRes2 = Crn.cum_finder(Res2);
                    Max = *max_element(CRes2.begin(), CRes2.end());
                    W << "Pashkeev_Less : " << Crn.Print(CRes2) << ": " << Max << " : " << N << " : " << M << '\n';

                    Res2 = Crn.croneProcess(Res2);
                    CRes2 = Crn.cum_finder(Res2);
                    Max = *max_element(CRes2.begin(), CRes2.end());
                    W << "Pashkeev_Less-Crone : " << Crn.Print(CRes2) << ": " << Max << " : " << N << " : " << M << '\n';

                    Res2 = Crn.Pashkeev(2);
                    CRes2 = Crn.cum_finder(Res2);
                    Max = *max_element(CRes2.begin(), CRes2.end());
                    W << "Pashkeev_Greater : " << Crn.Print(CRes2) << ": " << Max << " : " << N << " : " << M << '\n';

                    Res2 = Crn.croneProcess(Res2);
                    CRes2 = Crn.cum_finder(Res2);
                    Max = *max_element(CRes2.begin(), CRes2.end());
                    W << "Pashkeev_Greater-Crone : " << Crn.Print(CRes2) << ": " << Max << " : " << N << " : " << M << '\n';
                }
            }
        }
        T.close();
        W.close();
    }
}

int to_int(const std::string &s)
{
    std::istringstream iss(s);
    int value;
    iss >> value;
    if (iss.fail())
    {
        throw std::invalid_argument("Invalid integer value: " + s);
    }
    return value;
}

float to_float(const std::string &s)
{
    std::istringstream iss(s);
    float value;
    iss >> value;
    if (iss.fail())
    {
        throw std::invalid_argument("Invalid float value: " + s);
    }
    std::cout << value << '\n';
    return value;
}

// Функция для преобразования строки в вектор целых чисел
std::vector<int> to_int_vector(const std::string &s)
{
    std::vector<int> result;
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, ','))
    {
        result.push_back(to_int(item));
    }
    return result;
}

std::vector<float> to_float_vector(const std::string &s)
{
    std::vector<float> result;
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, ','))
    {
        result.push_back(to_float(item));
    }
    return result;
}

int main(int argc, char *argv[])
{
    if (argc < 6)
    {
        std::cerr << "Usage: " << argv[0] << " <int> <string> <vector of ints (comma-separated)>" << std::endl;
        return 1;
    }

    try
    {
        std::string fileName = argv[1];
        cout << fileName << '\n';
        std::vector<int> Ns = to_int_vector(argv[2]);
        std::vector<int> Ms = to_int_vector(argv[3]);
        std::vector<int> Interval = to_int_vector(argv[4]);
        int MAX = to_int(argv[5]);

        writeData(fileName, Ns, Ms, Interval, MAX);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // string fileName = "dataN.txt";
    // vector<int> Ns = {5, 10};
    // vector<int> Ms = {7, 100, 150, 200};
    // vector<int> Interval = {10, 20};
    // int MAX = 99;

    return 0;
}
