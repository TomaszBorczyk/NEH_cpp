#include <iostream>
#include <vector>
#include <algorithm>
#include "fstream"
#include <numeric>

//komparator do porównywania zadań wg ich sumy czasów na maszynach
//comparator for comparing job by their total time on machines
bool vecCompare(const std::vector<int>& firstElem, const std::vector<int>& secondElem) {
    return std::accumulate(firstElem.begin(), firstElem.end(), 0) > std::accumulate(secondElem.begin(), secondElem.end(), 0);

}

//funkcja sortująca zadania wg ich sumy czasu
//function sorting jobs by their total time
std::vector<std::vector<int>> sortJobs(std::vector<std::vector<int>> jobs){

    std::sort(jobs.begin(), jobs.end(), vecCompare);
    return jobs;
}


void displayJobs(std::vector<std::vector<int> > jobs){
    for(auto &vec : jobs){
        for (auto &job : vec){
            printf("%i ", job);
        }
        printf("sum: %i\n", std::accumulate(vec.begin(), vec.end(), 0));
    }
}

//funkcja licząca cmax dla zadanych zadań w podanej kolejności
//function evaluating cmax for jobs in given order
int evalV(std::vector<std::vector<int>> jobs){
    std::vector<int> machines(jobs[0].size(), 0);

    for(auto &job : jobs)
        for(int i=0; i<job.size(); i++){
            machines[i] = std::max(machines[i], i==0 ? -1 : machines[i-1]) + job[i];
        }
    return machines.back();
}


//funkcja szukająca najlepszego cmax dla problemu wielomaszynowego
//function looking for cmax in flow shop problem
int evalNEH(std::vector<std::vector<int>> jobs){
    std::vector<std::vector<int>> currPerm;
    currPerm.push_back(jobs[0]);
    currPerm.push_back(jobs[1]);
    int cmax;
    std::vector<std::vector<int>> perm = currPerm;

    for (int j = 2; j <= jobs.size(); j++) {
        cmax = INT_MAX;
        for(int i = 0; i<currPerm.size(); i++){

            int temp_cmax = evalV(currPerm);
            if(temp_cmax< cmax){
                cmax = temp_cmax;
                perm = currPerm;
            }
//            displayJobs(currPerm);
//            printf("%i\n", cmax);

            if(i<currPerm.size()-1)
                std::next_permutation(currPerm.end()-2-i, currPerm.end()-i);
//            printf_s("=====================\n");
        }
//        printf_s("oooooooooooooooooooooooooooo\n");

        currPerm = perm;

        if(j<jobs.size())
            currPerm.push_back(jobs[j]);
    }

    return cmax;
}

int main()
{
    std::vector<std::vector<int> > processingTimes;
    std::string filename = "neh_9_in.txt";
    std::fstream file;
    int n, m;
    int cMax = -1;


    file.open(filename);
    file >> n >> m;
    for (int i = 0; i < n; i++)
    {
        processingTimes.push_back(std::vector<int>());
        for (int j = 0; j < m; j++)
        {
            int p;
            file >> p;
            processingTimes[i].push_back(p);
        }
    }


//    displayJobs(processingTimes);
    processingTimes = sortJobs(processingTimes);

//    printf_s("=====================\n");
//    displayJobs(processingTimes);

    cMax = evalNEH(processingTimes);

    std::cout << cMax << std::endl;

    return 0;
}
