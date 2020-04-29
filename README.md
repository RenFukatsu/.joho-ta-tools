# .joho-ta-tools
testcase作成用のツール

## Install
```bash
cd $HOME
git clone https://github.com/RenFukatsu/.joho-ta-tools.git
cd joho-ta-tools
g++ joho-ta-tools.cpp -o joho-ta-tools
echo "PATH=PATH:path_to_joho-ta-tools-dir" >> ~/.profile
source ~/.profile
```

## How to use
1. テストケースの模範解答があるディレクトリに移動
2. `joho-ta-tools`でディレクトリ内の`main.cpp`がコンパイルされ、入力待ちの状態になる。
2'. `joho-ta-tools c/c++のfile`でディレクトリ内の`c/c++のfile`がコンパイルされ、入力待ちの状態になる。
3. `Input?:`に続いて、テストケースの入力を行う。入力が終わったら、`Enter`を入力。
4. `Output?:`として出力される。
5. テストケースの作成を終えたいときは、`Input?:`の時に`Enter`を入力する。
6. 生成物として、`num.in`, `num.out`, `testcase.zip`が生成される。