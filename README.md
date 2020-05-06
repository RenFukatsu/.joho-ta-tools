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
6. 生成物として、`num.in`, `num.out`, `testcase.zip`が生成される。

## How to use with special judge
上記の1, 3, 4, 5は同じ。

2'. `joho-ta-tools spj`でディレクトリ内の`main.cpp`がコンパイルされ、入力待ちの状態になる。

6'. `spj`をつけていた場合は、`num.in`, `testcase.zip`, `spj.cpp`が出力される。

## 注意
- spj採用の場合は`Create Problem`の画面で、spjのコンパイルをc++に指定する。
- 色々バグがあると思うんで、バグの再現例とissueかpull reqお願いします。
