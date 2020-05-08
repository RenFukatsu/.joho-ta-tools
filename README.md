# .joho-ta-tools
testcase作成用のツール

## Environment
- Ubuntu 18.04
- c++11以上

## Install
```bash
cd ~
git clone https://github.com/RenFukatsu/.joho-ta-tools.git
cd .joho-ta-tools
git clone https://github.com/SiLeader/argparse.git
g++ joho-ta-tools.cpp -o joho-ta-tools
echo "PATH=PATH:~/.joho-ta-tools" >> ~/.profile
source ~/.profile
```

## How to use
1. テストケースの模範解答(`main.cpp`)があるディレクトリに移動
2. `joho-ta-tools`でディレクトリ内の`main.cpp`がコンパイルされ、入力待ちの状態になる。
3. `Input?:`に続いて、テストケースの入力を行う。入力が終わったら、`Enter`を入力。
4. `Output?:`として、計算結果が出力される。
5. テストケースの作成を終えたいときは、`Input?:`の時に`Enter`を入力する。
6. 生成物として、`*.in`, `*.out`, `testcase.zip`が生成される。

## How to use with special judge
上記の1, 3, 4, 5は同じ。

2'. `joho-ta-tools -s`でディレクトリ内の`main.cpp`がコンパイルされ、入力待ちの状態になる。

6'. `*.in`, `testcase.zip`, `spj.cpp`が出力される。

## How to use with random mode
1. `joho-ta-tools -r num_of_samples`を入力
2. `Input format:`に続いて、入力の型を決定する。入力の際のルールは以下に従う。
   ```
   int : i min max
   long long : ll min max
   float : f min max
   double : d min max
   char : c min max
   string : s min max min_word_count max_word_count
   ```
   例1). float型で-100~100の範囲の入力を与えたいとき、
   ```
   f -100 100
   ```
   例2). char型でa~zの範囲の入力を与えたいとき、
   ```
   c a z
   ```
   例3). string型でA~zの範囲で1文字以上、100文字以下の入力を与えたいとき、
   ```
   s A z 1 100
   ```
   注意). 文字範囲の入力はASCIIコード表に準ずるため、`A z`の時、`[, \, ], etc`などの記号も含まれる。
3. inputとoutputが出力され、`*.in`, `*.out`, `testcase.zip`などが生成される。



## 注意
- spj採用の場合は`Create Problem`の画面で、spjのコンパイルをc++に指定する。
- 色々バグがあると思うんで、バグの再現例とissueかpull reqお願いします。
