import argparse
import os
import tqdm
import sys


class Submit:
    def __init__(self, file_path: str) -> None:
        self.problem_no = int(file_path[-6:-4])
        self.student = file_path[-17:-7]
        with open(file_path, 'r') as f:
            self.text = f.read()

class UnionFind:
    def __init__(self, n: int) -> None:
        self.n = n
        self.d = [-1] * n
    
    def find(self, x):
        parent = self.d[x]
        while parent >= 0:
            if self.d[parent] < 0:
                return parent
            self.d[x], x, parent = (
                self.d[parent],
                self.d[parent],
                self.d[self.d[parent]]
            )
        return x
    
    def unite(self, x, y):
        x = self.find(x)
        y = self.find(y)
        if x == y:
            return False
        if self.d[x] > self.d[y]:
            x, y = y, x
        self.d[x] += self.d[y]
        self.d[y] = x
        return True
    
    def same(self, x, y):
        fx = self.find(x)
        fy = self.find(y)
        return fx == fy
    
    def size(self, x):
        return -self.d[self.find(x)]
    
    def groups(self):
        find_buf = [self.find(i) for i in range(self.n)]
        res = [[] for _ in range(self.n)]
        for i in range(self.n):
            res[find_buf[i]].append(i)
        
        return list(filter(lambda r: r, res))
            

def main():
    parser = argparse.ArgumentParser(description='edit distance')
    parser.add_argument('ac_dir', help='required ac dir')
    parser.add_argument('-t', '--tolerance', type=int, default=10)
    parser.add_argument('-o', '--output', default='output.csv')
    args = parser.parse_args()

    submissions = parse_submission(args.ac_dir)
    problems = dict()
    for sub in submissions:
        problems.setdefault(sub.problem_no, []).append(sub)
    ans = []
    for no in problems:
        print("Problem" + str(no))
        groups = clustering(problems[no], args.tolerance)
        ans.append(groups)
    
    with open(args.output, 'w') as f:
        for i in range(len(ans)):
            f.write("Problem" + str(ans[i][0][0].problem_no) + '\n')
            for g in ans[i]:
                for st in g:
                    f.write(st.student + ',')
                f.write('\n')
            f.write('\n')

def parse_submission(ac_dir):
    res = list()
    files = os.listdir(ac_dir)
    for file in files:
        res.append(Submit(os.path.join(ac_dir, file)))
    return res;

def clustering(submissions, tolerance):
    uf = UnionFind(len(submissions))
    sz = len(submissions)
    for i in tqdm.tqdm(range(sz)):
        for j in range(i + 1, sz):
            edit_distance = calc_edit_distance(submissions[i].text, submissions[j].text, tolerance)
            if edit_distance < tolerance:
                uf.unite(i, j)
    grs = uf.groups()
    res = []
    for g in grs:
        if len(g) == 1:
            continue
        l = []
        for st in g:
            l.append(submissions[st])
        res.append(l)
    return res

def calc_edit_distance(str1: str, str2: str, tolerance: int):
    inf = 1<<29
    dp = [[inf] * (len(str2) + 1) for i in range(len(str1) + 1)]
    dp[0][0] = 0
    sz1 = len(str1)
    sz2 = len(str2)

    for i in range(sz1 + 1):
        mindp = 1<<29
        for j in range(sz2 + 1):
            # change
            if i > 0 and j > 0:
                if str1[i - 1] == str2[j - 1]:
                    dp[i][j] = min(dp[i][j], dp[i - 1][j - 1])
                else:
                    dp[i][j] = min(dp[i][j], dp[i - 1][j - 1] + 1)
            # erase
            if i > 0:
                # print(i, j)
                dp[i][j] = min(dp[i][j], dp[i - 1][j] + 1)
            # input
            if j > 0:
                dp[i][j] = min(dp[i][j], dp[i][j - 1] + 1)
            mindp = min(mindp, dp[i][j])
        if mindp > tolerance:
            return mindp
    
    return dp[len(str1)][len(str2)]


if __name__ == '__main__':
    main()
