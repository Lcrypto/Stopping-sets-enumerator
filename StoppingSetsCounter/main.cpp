#include<iostream>
#include<vector>
#include<queue>

#include "ClpSimplex.hpp"


using namespace std;


class StoppingSetFinder {
    const double EPS = 1e-8;
    int n, k;
    vector<vector<int> > TannerGraphCheck, TannerGraphVar, checkMtr;
    vector<double> linearProblemSolution;
public:
    StoppingSetFinder(const vector<vector<int> >& _h) {
        checkMtr = _h;
        k = checkMtr.size();
        n = checkMtr[0].size();
        TannerGraphCheck.resize(k);
        TannerGraphVar.resize(n);
        for (int i = 0; i < k; ++i) {
            for (int j = 0; j < n; ++j) {
                if (checkMtr[i][j]) {
                    TannerGraphCheck[i].push_back(j);
                    TannerGraphVar[j].push_back(i);
                }
            }
        }
    }
private:
    bool extendedDecoding(vector<int>& curSet) {
        vector<int> unknown(k, 0);
        for (int i = 0; i < k; ++i)
            unknown[i] = TannerGraphCheck.size();
        vector<int> sum(k, 0);
        for (int i = 0; i < n; ++i) {
            if (curSet[i] == -1)
                continue;
            for (int j = 0; j < TannerGraphVar[i].size(); ++j) {
                sum[TannerGraphVar[i][j]] += curSet[i];
                --unknown[TannerGraphVar[i][j]];
            }
        }
        queue<int> q;
        for (int i = 0; i < k; ++i) {
            if (unknown[i] == 0) {
                if (sum[i] == 1)
                    return 0;
            }
            if ((unknown[i] == 1) && (sum[i] < 2))
                q.push(i);
        }
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            if (unknown[cur] == 0) {
                if (sum[cur] == 1)
                    return 0;
                continue;
            }
            for (int i = 0; i < TannerGraphCheck[cur].size(); ++i) {
                int id = TannerGraphCheck[cur][i];
                if (curSet[id] == -1) {
                    curSet[id] = sum[cur];
                    sum[cur] *= 2;
                    for (int j = 0; j < TannerGraphVar[id].size(); ++j) {
                        int checkId = TannerGraphVar[id][j];
                        --unknown[checkId];
                        sum[checkId] += curSet[id];
                        if (unknown[checkId] == 0) {
                            if (sum[checkId] == 1)
                                return 0;
                        }
                        if ((unknown[checkId] == 1) && (sum[checkId] < 2)) {
                            q.push(checkId);
                        }
                    }
                    break;
                }
            }
        }
        return 1;
    }

    int getCard(const vector<int>& a) {
        int res = 0;
        for (int i = 0; i < a.size(); ++i)
            if (a[i] != -1)
                ++res;
        return res;
    }

    int getWeight(const vector<int>& a) {
        int res = 0;
        for (int i = 0; i < a.size(); ++i)
            if (a[i] == 1)
                ++res;
        return res;
    }

    bool isStoppingSet(const vector<int>& a) {
        bool ok = 0;
        for (int i = 0; i < a.size(); ++i) {
            if (a[i] == 1) {
                ok = 1;
                break;
            }
        }
        if (!ok)
            return 0;
        for (int i = 0; i < k; ++i) {
            int x = 0;
            for (int j = 0; j < TannerGraphCheck[i].size(); ++j)
                if (a[TannerGraphCheck[i][j]] == 1)
                    ++x;
            if (x == 1)
                return false;
        }
        return true;
    }

    int getLowerBound(const vector<int>& a) {
        ClpSimplex  model;
        return getWeight(a);



    }

    int choosePosWithMaxWeight(const vector<int>& a) {
        int res = -1;
        int id = -1;
        for (int i = 0; i < a.size(); ++i) {
            if (TannerGraphVar[a[i]].size() > res) {
                res = TannerGraphVar[a[i]].size();
                id = i;
            }
        }
        return id;
    }

    int chooseNewPos(const vector<int>& a) {
        vector<int> unusedInd;
        for (int i = 0; i < a.size(); ++i)
            if (a[i] == -1)
                unusedInd.push_back(i);
        return unusedInd[rand() % unusedInd.size()];

    }
    public:
    int findMinimumSizeOfStoppingSet() {
        int currentMin = 5;
        vector<int> minStoppingSet;
        queue<vector<int> > constraintSets;
        vector<int> erased(n, -1);
        constraintSets.push(erased);
        while (!constraintSets.empty()) {
            cerr << constraintSets.size() << endl;
            vector<int> curSet = constraintSets.front();
            constraintSets.pop();
            int weight = getWeight(curSet);
            if (weight >= currentMin)
                continue;
            if (!extendedDecoding(curSet))
                continue;
            if (getCard(curSet) == n) {
                if ((weight < currentMin) && (isStoppingSet(curSet))) {
                    currentMin = weight;
                    minStoppingSet = curSet;
                    cerr << currentMin << endl;
                }
            }
            else {
                if ((weight >= currentMin) || (getLowerBound(curSet) >= currentMin))
                    continue;
                int pos = chooseNewPos(curSet);
                vector<int> newSet0(curSet.begin(), curSet.end());
                vector<int> newSet1(curSet.begin(), curSet.end());
                newSet0[pos] = 0;
                newSet1[pos] = 1;
                constraintSets.push(newSet0);
                constraintSets.push(newSet1);
            }
        }
        return currentMin;
    }

};

void test() {
    freopen("testInput.txt", "r", stdin);
    int n, k, p;
    cin >> n >> k >> p;
    vector<vector<int> > checkMtr(k * p, vector<int>(n * p, 0));
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < n; ++j) {
            int x;
            cin >> x;
            for (int l = 0; l < p; ++l)
                checkMtr[i * p + l][j * p + (l + x) % p] = 1;
        }
    }
    StoppingSetFinder finder(checkMtr);
    cout << finder.findMinimumSizeOfStoppingSet() << endl;
    return;
}

int main(int argc, char* argv[]) {
    test();
    return 0;
    string INPUT_FILENAME = "";
    for (int i = 1; i + 1 < argc; ++i) {
        if (string(argv[i]) == "-file") {
            INPUT_FILENAME = argv[i + 1];
            ++i;
            continue;
        }
    }
    freopen(INPUT_FILENAME.c_str(), "r", stdin);
    int n, k, p;
    cin >> n >> k >> p;
    vector<vector<int> > checkMtr(k * p, vector<int>(n * p, 0));
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < n; ++j) {
            int x;
            cin >> x;
            for (int l = 0; l < p; ++l)
                checkMtr[i * p + l][j * p + (l + x) % p] = 1;
        }
    }
    StoppingSetFinder finder(checkMtr);
    cout << finder.findMinimumSizeOfStoppingSet() << endl;
    return 0;
}
