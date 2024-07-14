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

double randomfloat(float t1, float t2)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(t1, t2);
    // cout << dis(gen) << '\n';
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
        for (int i = 0; i <= M; ++i)
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

    std::string Print(vector<vector<int>>& Darvin)
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

    std::string Print(vector<int>& OBJECTect)
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
        cout << "HERE IS SHIT\n";
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

class Genom
{
private:
    int N;
    int M;
    int Z;
    int a;
    int b;
    double crossover_probability, mutation_probability;
    vector<int> INTERVAL;
    vector<int> TDeterminade = {};
    bool isDeterminade = false;
    vector<int> index;

public:
    Genom(int N, int M, int a, int b, int Z, double crossover_probability, double mutation_probability, bool isDeterminade = false)
    {
        this->N = N;
        this->M = M - 1;
        this->a = a;
        this->b = b;
        this->Z = Z;
        this->crossover_probability = crossover_probability;
        this->mutation_probability = mutation_probability;
        this->isDeterminade = isDeterminade;
        INTERVAL = divide_interval();
        if (isDeterminade)
            MakeTForDeterminade();
    }

    string projection;
    string endProjection;
    vector<int> bestObject;

    void MakeTForDeterminade()
    {
        for (int i = 0; i <= M; ++i)
            TDeterminade.push_back(random(a, b));
    }

    void setTDeterminade(vector<int> T)
    {
        TDeterminade = T;
    }

    std::vector<int> divide_interval()
    {
        const int interval_start = 0;
        const int interval_end = 256;
        const int interval_size = interval_end - interval_start;

        // Вычисляем размер каждой части
        int part_size = interval_size / N;

        std::vector<int> interval_parts;
        interval_parts.push_back(interval_start);

        for (int i = 1; i < N; ++i)
        {
            int part_boundary = interval_start + i * part_size;
            interval_parts.push_back(part_boundary);
        }

        interval_parts.push_back(interval_end);
        return interval_parts;
    }

    vector<int> Helper(vector<int> T0, int MOD)
    {
        Crone OBJECT(N, M, T0);
        vector<int> Single(M + 1, 0);
        vector<vector<int>> columns;

        switch (MOD)
        {
        case 0:
            columns = OBJECT.croneProcess();
            break;
        case 1:
            columns = OBJECT.croneProcessModified();
            break;
        case 2:
            columns = OBJECT.criticalPath();
            break;
        case 3:
            columns = OBJECT.Pashkeev();
            break;
        }

        // int cnt = 0;
        // for (int i = 0; i < columns.size(); ++i) {
        //     cnt += columns[i].size();
        // }
        // cout << "SIZE SHIT IS " << cnt << ' ' << T0.size() << '\n';

        // int zero_counter = 0;
        // while (zero_counter != M + 1) {

        // int i = random(0, N - 1);
        // int j = random(0, (int)columns[i].size() - 1);

        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < (int)columns[i].size(); ++j)
            {

                for (int k = 0; k <= M; ++k)
                {
                    // cout << T0[k] << ' ' << columns[i][j] << '\n';
                    if (T0[k] == columns[i][j])
                    {

                        int interval_random = random(INTERVAL[i], INTERVAL[i + 1] - 1);
                        string T_8bit = intToBinary(columns[i][j]);
                        string interval_binary = intToBinary(interval_random);
                        string codec = T_8bit + interval_binary;
                        T0[k] = -999;
                        Single[k] = binaryToInt(codec);
                        /*  ++zero_counter;*/
                        break;
                    }
                }
            }
        }
        return Single;
    }

    vector<vector<int>> first_generation(bool modify, int chose_alg, int elite_number)
    {
        vector<int> T = {};
        int k = 0;
        if (isDeterminade)
            T = TDeterminade;
        else
            for (int i = 0; i <= M; ++i)
                T.push_back(random(a, b));

        vector<vector<int>> Darvin = {};
        if (modify)
        {
            for (int i = 0; i <= elite_number; ++i)
            {
                ++k;
                Darvin.push_back(Helper(T, chose_alg));
                cout << "Phen of elites " << Phenotype(Darvin[i]) << '\n';
            }
        }

        for (int i = k; i < Z; ++i)
        {
            vector<int> Single;
            index.push_back(i);
            for (int j = 0; j <= M; ++j)
            {
                int choice = random(0, N - 1);
                int interval_random = random(INTERVAL[choice], INTERVAL[choice + 1] - 1);

                string T_8bit = intToBinary(T[j]);
                string interval_binary = intToBinary(interval_random);
                string codec = T_8bit + interval_binary;

                Single.push_back(binaryToInt(codec));
            }
            Darvin.push_back(Single);
            //cout << "Phen of one " << Phenotype(Darvin[i]) << '\n';
        }
        return Darvin;
    }

    // Перегрузка для метода без элит!!!
    vector<vector<int>> first_generation()
    {
        vector<int> T = {};
        int k = 0;
        if (isDeterminade)
            T = TDeterminade;
        else
            for (int i = 0; i <= M; ++i)
                T.push_back(random(a, b));

        vector<vector<int>> Darvin = {};

        for (int i = k; i < Z; ++i)
        {
            vector<int> Single;
            index.push_back(i);
            for (int j = 0; j <= M; ++j)
            {
                int choice = random(0, N - 1);
                int interval_random = random(INTERVAL[choice], INTERVAL[choice + 1] - 1);

                string T_8bit = intToBinary(T[j]);
                string interval_binary = intToBinary(interval_random);
                string codec = T_8bit + interval_binary;

                Single.push_back(binaryToInt(codec));
            }
            Darvin.push_back(Single);
        }
        return Darvin;
    }

    int Phenotype(vector<int> One)
    {
        vector<int> phenotype(N, 0);
        for (int i = 0; i <= M; ++i)
        {
            int diapazone = getInterval(One[i]);
            // if (diapazone == 0) {
            //     cout << "WE FUCKED SO BAD! \n";
            // }
            int value = getValue(One[i]);

            // cout << N << '\n';
            for (int j = 0; j < N; ++j)
            {
                // cout << j << '\n';
                // cout << INTERVAL[j] << " " << INTERVAL[j+1] << '\n';
                if (INTERVAL[j] <= diapazone && INTERVAL[j + 1] > diapazone)
                {
                    phenotype[j] += value;
                }
            }
        }
        int max_value = (int)*max_element(phenotype.begin(), phenotype.end());
        return max_value;
    }

    vector<vector<int>> ConvertorGoldToCrone(vector<int> One)
    {
        vector<vector<int>> columns(N, vector<int>());
        for (int i = 0; i <= M; ++i)
        {
            int diapazone = getInterval(One[i]);
            int value = getValue(One[i]);

            for (int j = 0; j < N; ++j)
            {
                if (INTERVAL[j] <= diapazone && INTERVAL[j + 1] > diapazone)
                {
                    columns[j].push_back(value);
                    /*                   cout << j << '\n';
                                       cout << INTERVAL[j] <<' ' << INTERVAL[j+1] << '\n';*/
                }
            }
        }
        return columns;
    }

    vector<int> Mutation_operation(vector<int> Subject)
    {
        int random_mutate_part = random(0, M);
        int hromosome_point = random(0, 7);
        string part = intToBinary16(Subject[random_mutate_part]).substr(8, 8);
        string value = intToBinary16(Subject[random_mutate_part]).substr(0, 8);
        // cout << random_mutate_part << ' ' << Subject[random_mutate_part] << ' ' << part << ' ' << value << '\n';
        (part[hromosome_point] == '0') ? part[hromosome_point] = '1' : part[hromosome_point] = '0';
        if (Subject[random_mutate_part] == 0)
            cout << "WHAT THE FUCK\n";
        string result = value + part;
        Subject[random_mutate_part] = binaryToInt(result);
        // cout << part << ' ' << Subject[random_mutate_part] << endl;

        return Subject;
    }

    vector<int> Crossover_operation(vector<int> first_parent, vector<int> second_parent)
    {
        int border_for_cross = random(1, M - 1);
        // cout << "start fuck\n";
        int fparent_phenotype = Phenotype(first_parent);
        int sparent_phenotype = Phenotype(second_parent);
        // cout << "start fuck333\n";
        // cout << "border of crossover from " << border_for_cross << " to " << M << endl;
        // cout << "Phenotype of 2 parents: " << fparent_phenotype << ' ' << sparent_phenotype << endl;
        vector<int> child_one, child_two;
        for (int i = 0; i <= M; ++i)
        {
            if (i < border_for_cross)
            {
                child_one.push_back(first_parent[i]);
                child_two.push_back(second_parent[i]);
            }
            else
            {
                child_one.push_back(second_parent[i]);
                child_two.push_back(first_parent[i]);
            }
        }
        if (randomfloat(0, 1) <= mutation_probability)
        {
            child_one = Mutation_operation(child_one);
            child_two = Mutation_operation(child_two);
        }

        int child_one_phenotype = Phenotype(child_one);
        int child_two_phenotype = Phenotype(child_two);
        int winner;
        if (child_one_phenotype < child_two_phenotype)
        {
            winner = child_one_phenotype;
            if (winner < fparent_phenotype)
            {
                // cout << winner << ' ' << "--------First child is a sucsessor--------" << endl;
                return child_one;
            }
            else
            {
                return vector<int>{};
            }
        }
        else
        {
            winner = child_two_phenotype;
            if (winner < fparent_phenotype)
            {
                // cout << winner << ' ' << "--------Second child is a sucsessor--------" << endl;
                return child_two;
            }
            else
            {
                return vector<int>{};
            }
        }
    }

    // Перегрузка для чистого Голдберга без элит
    vector<int> EliteModule(int stop_count, vector<vector<int>> Darvin)
    {
        auto start = std::chrono::steady_clock::now();
        int end_counter = 0;
        int memory_value = 1000000;
        int load_memory = 0;
        int count = 0;
        int positionInPopulation = 0;
        while (end_counter < stop_count)
        {
            if (count == 0)
            {
                load_memory = Phenotype(Darvin[0]);
            }
            vector<vector<int>> Darvin_copy = Darvin;
            for (int i = 0; i < Z; ++i)
            {
                int j = random(0, Z - 1);
                while (j == i)
                    j = random(0, Z - 1);

                if (randomfloat(0, 1) <= crossover_probability)
                {
                    vector<int> tmp = Crossover_operation(Darvin[i], Darvin[j]);
                    if (!(tmp.empty()))
                        Darvin_copy[i] = tmp;
                }
                else if (randomfloat(0, 1) <= mutation_probability)
                {
                    cout << "somehow mutation happend...\n";
                    Darvin_copy[i] = Mutation_operation(Darvin[i]);
                }
            }
            Darvin = Darvin_copy;
            Darvin_copy.clear();

            int min = 1000000;
            for (int i = 0; i < Z; ++i)
            {
                int x = Phenotype(Darvin[i]);
                if (min > x)
                {
                    min = x;
                    positionInPopulation = i;
                }
            }

            if (memory_value == min)
            {
                end_counter++;
            }
            else
            {
                end_counter = 1;
            }

            memory_value = min;
            count++;

            //cout << "Min value: " << memory_value << ", End counter: " << end_counter << ", Generation number: " << count << endl;
        }

        cout << "Min value: " << memory_value << ", End counter: " << end_counter << ", Generation number: " << count << endl;
        PrintVector(INTERVAL);

        cout << endl;
        endProjection = PrintObject(Darvin, positionInPopulation);
        bestObject = Darvin[positionInPopulation];
        auto end = std::chrono::steady_clock::now();
        int Time = (std::chrono::duration_cast<std::chrono::milliseconds>(end - start)).count();

        // Print(Darvin);
        return vector<int>{memory_value, load_memory, count};
    }

    void PrintVector(vector<int> arr)
    {
        for (int i = 0; i < arr.size(); ++i)
            cout << arr[i] << ' ';
        cout << '\n';
    }

    // Помогает мне внедрить конвертированный в столбцы лучшую особь Голдберга в алгоритм Крона
    int GoldbergCroneResult()
    {
        // cout << "len of best is" << bestObject.size() << '\n';
        vector<vector<int>> tmp = ConvertorGoldToCrone(bestObject);
        Crone OBJECT(N, M, TDeterminade);
        OBJECT.Print(tmp);
        cout << "----------------\n";
        vector<vector<int>> tmp2 = OBJECT.croneProcess(tmp);
        int value = OBJECT.MaxCumFinder(OBJECT.cum_finder(tmp2));

        return value;
    }

    int CroneResult(vector<int> One)
    {
        // cout << "len of best is" << bestObject.size() << '\n';
        vector<vector<int>> tmp = ConvertorGoldToCrone(One);
        Crone OBJECT(N, M, TDeterminade);
        // cout << "-------\n";
        // OBJECT.Print(tmp);
        // vector<vector<int>> tmp2 = OBJECT.croneProcess(tmp);
        int value = OBJECT.MaxCumFinder(OBJECT.cum_finder(tmp));

        return value;
    }

    void Print(vector<vector<int>>& Darvin)
    {
        for (int i = 0; i < Z; ++i)
        {
            cout << PrintObject(Darvin, i);
            cout << "\n \n";
        }
    }

    vector<vector<int>> MAJORSORT(vector<vector<int>>& list, int BORDER)
    {
        int elite_border = BORDER + 1;
        for (int step = 1; step < elite_border; step++)
        {
            int j = step - 1;
            vector<int> key = list[step];
            int keyIndex = index[step];

            int Pkey = Phenotype(key);
            int PVal = Phenotype(list[j]);
            while (Pkey < PVal && j >= 0)
            {
                PVal = Phenotype(list[j]);
                list[j + 1] = list[j];
                index[j + 1] = index[j];
                --j;
            }
            list[j + 1] = key;
            index[j + 1] = keyIndex;
        }
        int MAXPhenotype = 100000000;
        bool isfind = false;
        int pos = 0;
        for (int i = BORDER + 1; i < list.size(); ++i)
            if (MAXPhenotype > Phenotype(list[i]))
            {
                MAXPhenotype = Phenotype(list[i]);
                isfind = true;
                pos = i;
                // break;
            }
        if (isfind && (MAXPhenotype < Phenotype(list[BORDER])))
        {
            swap(list[pos], list[BORDER]);
            swap(index[pos], index[BORDER]);
        }

        return list;
    }

    vector<int> EliteModule(int stop_count, vector<vector<int>> Darvin, int elite_border)
    {
        auto start = std::chrono::steady_clock::now();
        int end_counter = 0;
        int memory_value = 1000000;
        int load_memory = 0;
        int count = 0;
        int positionInPopulation = 0;
        vector<vector<int>> list = Darvin;
        while (end_counter < stop_count)
        {

            MAJORSORT(Darvin, elite_border);

            if (count == 0)
            {
                load_memory = Phenotype(Darvin[0]);
            }
            vector<vector<int>> Darvin_copy = Darvin;
            for (int i = 0; i < Z; ++i)
            {
                int j = random(elite_border, Z - 1);
                while (j == i)
                    j = random(elite_border, Z - 1);

                if (randomfloat(0, 1) <= crossover_probability)
                {
                    vector<int> tmp = Crossover_operation(Darvin[i], Darvin[j]);
                    if (!(tmp.empty()))
                    {
                        if (i > elite_border)
                            Darvin_copy[i] = tmp;
                        else if (j > elite_border)
                            Darvin_copy[j] = tmp;
                    }
                }
                else if (randomfloat(0, 1) <= mutation_probability)
                    if (i > elite_border)
                        Darvin_copy[i] = Mutation_operation(Darvin[i]);
            }
            Darvin = Darvin_copy;
            Darvin_copy.clear();

            int min = 1000000;
            for (int i = 0; i < Z; ++i)
            {
                int x = Phenotype(Darvin[i]);
                if (min > x)
                {
                    min = x;
                    positionInPopulation = i;
                }
            }

            if (memory_value == min)
            {
                end_counter++;
            }
            else
            {
                end_counter = 1;
            }
            memory_value = min;
            count++;
            //cout << "Min value: " << memory_value << ", End counter: " << end_counter << ", Generation number: " << count << endl;
            //  Print(Darvin);
        }

        cout << "Min value: " << memory_value << ", End counter: " << end_counter << ", Generation number: " << count << endl;
        cout << endl;
        auto end = std::chrono::steady_clock::now();
        int Time = (std::chrono::duration_cast<std::chrono::milliseconds>(end - start)).count();
        endProjection = PrintObject(Darvin, positionInPopulation);
        // cout << projection << "\n \n";
        // cout << endProjection << '\n';
        return vector<int>{memory_value, load_memory, count, Time};
    }

    string PrintObject(vector<vector<int>> BigObject, int X)
    {
        string text = "";
        vector<int> object = BigObject[X];
        for (int i = 0; i <= M; ++i)
        {
            string value = to_string(getValue(object[i]));
            string diapazone = to_string(getInterval(object[i]));
            text += value + ":" + diapazone + " ";
        }
        return text;
    }
};

void Programm()
{
    Genom Gen(10, 99, 10, 40, 1000, 1.0, 1.0, true);

    vector<vector<int>> proto = Gen.first_generation(true, 0, 0);
    Gen.EliteModule(1000, proto, 0);
    cout << Gen.endProjection << '\n';

    vector<vector<int>> proto1 = Gen.first_generation(true, 1, 0);
    Gen.EliteModule(1000, proto1, 0);
    cout << Gen.endProjection << '\n';

    vector<vector<int>> proto2 = Gen.first_generation(true, 2, 0);
    Gen.EliteModule(1000, proto2, 0);
    cout << Gen.endProjection << '\n';

    vector<vector<int>> proto3 = Gen.first_generation();
    Gen.EliteModule(1000, proto3);
    cout << Gen.endProjection << '\n';
    // int ToWriteGC = Gen.GoldbergCroneResult();
}

std::mutex fileMutex; // Создаем блокировку для безопасной записи в файл

void inputIntoFile(std::ofstream& outFile, std::string name, std::vector<int> data, int Z, int Percent, int N, int M)
{
    std::lock_guard<std::mutex> lk(fileMutex);
    outFile << name << " : " << Z << ", " << Percent << ", ";
    outFile << data[1] << ", " << data[2] << ", " << data[3] << " : " << data[0] << " : " << N << " : " << M << '\n';
}

void processData(std::ofstream& outFile, int Z, int Limit, int Iter, int threadId, int Nf, vector<int> Ms, int a, int b)
{
    // std::ofstream outFile(to_string(Z) + "_" + to_string(threadId) + "_" + to_string(N) + "_" + to_string(M) + FNType); // Используем уникальное имя файла для каждого потока
    if (outFile.is_open())
    {
        cout << "end";
        for (int M : Ms)
        {
            for (int N = 5; N <= Nf; ++N)
            {
                for (int j = 0; j < Iter; ++j)
                {
                    double crossover_p = 1, mutatuion_p = 1;
                    Genom Gen(N, M, a, b, Z, crossover_p, mutatuion_p, true);

                    for (int i = 0; i < Limit; ++i)
                    {
                        int Percent = ((double)Z * ((double)i / 10));

                        std::vector<std::vector<int>> clearGoldber = Gen.first_generation();
                        std::vector<std::vector<int>> proto = Gen.first_generation(true, 0, Percent);
                        std::vector<std::vector<int>> protoModified = Gen.first_generation(true, 1, Percent);
                        std::vector<std::vector<int>> protoCritical = Gen.first_generation(true, 2, Percent);
                        // std::vector<std::vector<int>> protoAllCrone = Gen.first_generation(true, true, Z - 1);

                        auto startG = std::chrono::steady_clock::now();
                        cout << "GOLDBERG VARIANT FOR Z " << Z << " AND ELITE BORDER [NONE] " << Percent << '\n';
                        std::vector<int> ToWrite0 = Gen.EliteModule(Z, clearGoldber);
                        auto endG = std::chrono::steady_clock::now();
                        auto GoldbergTime = std::chrono::duration_cast<std::chrono::milliseconds>(endG - startG);
                        cout << Gen.endProjection << '\n';

                        auto startGC = std::chrono::steady_clock::now();
                        cout << "GOLDBERG-CRONE VARIANT FOR Z " << Z << " AND ELITE BORDER [NONE] " << Percent << '\n';
                        int ToWriteGC = Gen.GoldbergCroneResult();
                        int CroneFromElite = Gen.CroneResult(proto[0]);
                        cout << "Min value for pure Crone: " << CroneFromElite << '\n';
                        cout << "Min value: " << ToWriteGC << '\n';
                        // cout << Gen.endProjection << '\n';
                        auto endGC = std::chrono::steady_clock::now();
                        auto GoldbergCroneTime = std::chrono::duration_cast<std::chrono::milliseconds>(endGC - startGC + GoldbergTime);

                        auto startC = std::chrono::steady_clock::now();
                        cout << "CRONE VARIANT FOR Z " << Z << " AND ELITE BORDER " << Percent << '\n';
                        std::vector<int> ToWrite = Gen.EliteModule(Z, proto, Percent);
                        auto endC = std::chrono::steady_clock::now();
                        auto GoldbergCroneEliteTime = std::chrono::duration_cast<std::chrono::milliseconds>(endC - startC);

                        auto startCM = std::chrono::steady_clock::now();
                        cout << "CRONE MODIFIED VARIANT FOR Z " << Z << " AND ELITE BORDER " << Percent << '\n';
                        std::vector<int> ToWriteCM = Gen.EliteModule(Z, protoModified, Percent);
                        auto endCM = std::chrono::steady_clock::now();
                        auto CroneModified = std::chrono::duration_cast<std::chrono::milliseconds>(endCM - startCM);

                        auto startCrit = std::chrono::steady_clock::now();
                        cout << "CRITICAL PATH VARIANT FOR Z " << Z << " AND ELITE BORDER " << Percent << '\n';
                        std::vector<int> ToWrite2 = Gen.EliteModule(Z, protoCritical, Percent);
                        auto endCrit = std::chrono::steady_clock::now();
                        auto GoldbergCriticalTime = std::chrono::duration_cast<std::chrono::milliseconds>(endCrit - startCrit);

                        // Захватываем блокировку только для записи в файл
                        {
                            std::lock_guard<std::mutex> lock(fileMutex);

                            inputIntoFile(outFile, "Goldberg", ToWrite0, Z, Percent, N, M);
                            ToWrite0[0] = CroneFromElite;
                            inputIntoFile(outFile, "Crone", ToWrite0, Z, Percent, N, M);

                            ToWrite0[0] = ToWriteGC;
                            inputIntoFile(outFile, "Goldberg-Crone", ToWrite0, Z, Percent, N, M);

                            inputIntoFile(outFile, "Goldberg with Elite-Crone", ToWrite, Z, Percent, N, M);

                            inputIntoFile(outFile, "Goldberg with Elite-Crone_Mod", ToWriteCM, Z, Percent, N, M);

                            inputIntoFile(outFile, "Goldberg with Elite-Critical_Path", ToWrite2, Z, Percent, N, M);
                        }

                        std::cout << "Iteration: " << i << "/" << Limit << ", Thread ID: " << threadId << ", Outer loop: " << j << '/' << Iter << std::endl;
                    }
                }
            }
        }
        cout << "end";
        outFile.close();
    }
}

void writeDataToFile(std::string name)
{
    int a = 10, b = 20;
    std::vector<int> Test_Z = { 100 };
    int Limit = 1;
    int Iter = 100;
    vector<int> Ms = { 70, 100, 150 };
    int Nf = 10;
    std::ofstream outFile(name);

    for (int i = 0; i < Test_Z.size(); ++i)
    {
        processData(outFile, Test_Z[i], Limit, Iter, i, Nf, Ms, a, b);
    }
}

void uniqueRun(std::ofstream& outFile, vector<int> mainValues, vector<int> T)
{
    int N = mainValues[0];
    int M = mainValues[1];
    int a = mainValues[3];
    int b = mainValues[4];
    int Z = mainValues[5];

    Genom Gen(N, M, a, b, Z, 1, 1, true);
    Gen.setTDeterminade(T);
    vector<vector<int>> goldbergGen = Gen.first_generation();
    vector<vector<int>> croneEliteGen = Gen.first_generation(true, 0, 0);
    vector<vector<int>> croneModEliteGen = Gen.first_generation(true, 1, 0);
    vector<vector<int>> criticalPathEliteGen = Gen.first_generation(true, 2, 0);
    vector<vector<int>> PashkeevEliteGen = Gen.first_generation(true, 3, 0);

    vector<int> resForGoldberg = Gen.EliteModule(Z, goldbergGen);
    inputIntoFile(outFile, "Goldberg", resForGoldberg, Z, 0, N, M);

    int croneAfterGoldberg = Gen.GoldbergCroneResult();
    resForGoldberg[0] = croneAfterGoldberg;
    inputIntoFile(outFile, "Crone_After_Goldberg", resForGoldberg, Z, 0, N, M);

    vector<string> names = { "GoldbergElite-Crone", "GoldbergElite-CroneMod", "GoldbergElite-CriticalPath", "GoldbergElite-Pashkeev" };
    vector<vector<vector<int>>> allGens = { croneEliteGen, croneModEliteGen, criticalPathEliteGen, PashkeevEliteGen };
    for (int i = 0; i < allGens.size(); ++i)
    {
        vector<int> res = Gen.EliteModule(Z, allGens[i], 0);
        inputIntoFile(outFile, names[i], res, Z, 0, N, M);
    }
}


void uniqueRunThreads(std::ofstream& outFile, vector<int> mainValues, vector<int> T)
{
    int N = mainValues[0];
    int M = mainValues[1];
    int a = mainValues[3];
    int b = mainValues[4];
    int Z = mainValues[5];

    Genom Gen(N, M, a, b, Z, 1, 1, true);
    Gen.setTDeterminade(T);
    vector<vector<int>> goldbergGen = Gen.first_generation();
    vector<vector<int>> croneEliteGen = Gen.first_generation(true, 0, 0);
    vector<vector<int>> croneModEliteGen = Gen.first_generation(true, 1, 0);
    vector<vector<int>> criticalPathEliteGen = Gen.first_generation(true, 2, 0);
    vector<vector<int>> PashkeevEliteGen = Gen.first_generation(true, 3, 0);

    std::thread GoldbergAndGC(
        [&]() {
            cout << "Thread: " << std::hash<std::thread::id>{}(std::this_thread::get_id()) << '\n';
            Genom GenThread = Gen;
            vector<int> resForGoldberg = GenThread.EliteModule(Z, goldbergGen);
            inputIntoFile(outFile, "Goldberg", resForGoldberg, Z, 0, N, M);
            int croneAfterGoldberg = GenThread.GoldbergCroneResult();
            resForGoldberg[0] = croneAfterGoldberg;
            inputIntoFile(outFile, "Crone_After_Goldberg", resForGoldberg, Z, 0, N, M);
        }
    );

    std::thread GoldbergEliteCrone(
        [&]() {
            cout << "Thread: " << std::hash<std::thread::id>{}(std::this_thread::get_id()) << '\n';
            Genom GenThread = Gen;
            vector<int> res = GenThread.EliteModule(Z, croneEliteGen, 0);
            inputIntoFile(outFile, "GoldbergElite-Crone", res, Z, 0, N, M);
        }
    );

    std::thread  GoldbergEliteCroneMod(
        [&]() {
            cout << "Thread: " << std::hash<std::thread::id>{}(std::this_thread::get_id()) << '\n';
            Genom GenThread = Gen;
            vector<int> res = GenThread.EliteModule(Z, croneModEliteGen, 0);
            inputIntoFile(outFile, "GoldbergElite-CroneMod", res, Z, 0, N, M);
        }
    );

    std::thread  GoldbergEliteCriticalPath(
        [&]() {
            cout << "Thread: " << std::hash<std::thread::id>{}(std::this_thread::get_id()) << '\n';
            Genom GenThread = Gen;
            vector<int> res = GenThread.EliteModule(Z, criticalPathEliteGen, 0);
            inputIntoFile(outFile, "GoldbergElite-CriticalPath", res, Z, 0, N, M);
        }
    );

    std::thread  GoldbergElitePashkeev(
        [&]() {
            cout << "Thread: " << std::hash<std::thread::id>{}(std::this_thread::get_id()) << '\n';
            Genom GenThread = Gen;
            vector<int> res = GenThread.EliteModule(Z, PashkeevEliteGen, 0);
            inputIntoFile(outFile, "GoldbergElite-Pashkeev", res, Z, 0, N, M);
        }
    );

    GoldbergAndGC.join();
    GoldbergEliteCrone.join();
    GoldbergEliteCroneMod.join();
    GoldbergEliteCriticalPath.join();
    GoldbergElitePashkeev.join();

}


void getValuesFromFile(std::string fileName, std::string writeFile, int Z, std::vector<int> Interval)
{
    ofstream outFile(writeFile);
    if (outFile.is_open())
    {
        outFile << '#' << fileName << '\n';

        ifstream inFile(fileName);
        string sPassWord;
        while (std::getline(inFile, sPassWord))
        {
            vector<int> getT = {};
            vector<int> otherValues = {};
            int positionAfterArray = 0;
            if (sPassWord[0] == '[')
            {
                std::string arr = "";
                for (int i = 1; i < sPassWord.size() && sPassWord[i] != ']'; ++i)
                {
                    positionAfterArray = i;
                    if (sPassWord[i] != ',')
                        arr += sPassWord[i];
                }
                std::string arr_2 = "";
                positionAfterArray += 3;
                for (int i = positionAfterArray; i < sPassWord.size(); ++i)
                {
                    if (sPassWord[i] != ',')
                        arr_2 += sPassWord[i];
                }
                std::stringstream makeArray(arr);
                std::stringstream makeAnotherArray(arr_2);
                int number;
                while (makeArray >> number)
                {
                    getT.push_back(number);
                }
                while (makeAnotherArray >> number)
                {
                    otherValues.push_back(number);
                }

                // if its Gleb alg then +1
                //otherValues[1] += 1;

                otherValues.push_back(Interval[0]);
                otherValues.push_back(Interval[1]);
                otherValues.push_back(Z);
                cout << "wtf " << getT.size() << ' ' << otherValues[1];
                // uniqueRun(outFile, otherValues, getT);
                uniqueRunThreads(outFile, otherValues, getT);
            }

            // there goes everything
        }
        outFile.close();
    }
}

int to_int(const std::string& s)
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

float to_float(const std::string& s)
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
std::vector<int> to_int_vector(const std::string& s)
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

std::vector<float> to_float_vector(const std::string& s)
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

int main(int argc, char* argv[])
{
    if (argc < 5)
    {
        std::cerr << "Usage: " << argv[0] << " <int> <string> <vector of ints (comma-separated)>" << std::endl;
        return 1;
    }

    try
    {
        cout << "start process \n";
        string TDir = "T/";
        string DloadTasks = "DownloadedTasks/";
        string dataDir = "GenData/";

        std::string readFile = TDir + argv[1];
        std::string makeFile = dataDir + argv[2];
        cout << makeFile << '\n';
        //  std::vector<int> Interval = to_int_vector(argv[5]);
        std::vector<int> Interval = { 10, 20 };
        int Z = to_int(argv[3]);
        int choseResDir = to_int(argv[4]);
        if (choseResDir == 1)
            readFile = DloadTasks + argv[1];
        getValuesFromFile(readFile, makeFile, Z, Interval);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // Programm();
    //  -----------------------mod false or true-- Crit is false - Crone is true -- empty T -- now much to modify
    /*   writeDataToFile();*/
    // writeDataToFile("GenData/gen_1.txt");
     //getValuesFromFile("T_data-2024-05-23_00-11-30_M80.txt", "gen_100_3.txt", 100, { 10, 20 });

    return 0;
}