# .joho-ta-tools
Online JudgeにおけるTAの仕事の効率化を目指します。
testcase作成の自動化ツールの`gen`コマンドとOJの結果を統合するための`merge`コマンドを用意しています。

## Environment
- Ubuntu 18.04
- c++11以上

## Install
```bash
cd ~
git clone https://github.com/RenFukatsu/.joho-ta-tools.git
cd .joho-ta-tools
git clone https://github.com/SiLeader/argparse.git
mkdir bin
g++ joho-ta-tools.cpp -o bin/joho-ta-tools
echo "export PATH=\"\$PATH:\$HOME/.joho-ta-tools\"" >> ~/.bashrc
source ~/.bashrc
```

## generate testcase
testcase作成の自動化ツール。模範解答(.c/.cpp)は手元にあること前提でそこからテストケースを半自動作成します。

### Folder Structure
基本のフォルダ構成は以下のようにしてください。

![Screenshot from 2021-04-08 19-10-12](https://user-images.githubusercontent.com/47164533/114009434-19339e00-989e-11eb-8fa6-900e4ce0ea4f.png)


### How to use
![Screenshot from 2021-04-08 19-12-53](https://user-images.githubusercontent.com/47164533/114009799-72033680-989e-11eb-9db0-66de39ebdd8e.png)

`joho-ta-tools gen -r num -f model_answer.cpp`で`model_answer.cpp`に対するテストケースがnum個できます。

`Input format:`に続いて、入力の型を決定します。入力の際のルールは以下に従い、入力してください。改行二回で入力を終了します。
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
   例4). int型で-100~100の範囲の入力を2つ横に並べて与えたいとき、
   ```
   i -100 100 i -100 100
   ```

最終的なフォルダ構成は以下のようになります。`zips/hoge.zip`をojの`Testcase choose file`というところに入れてください。
![Screenshot from 2021-04-08 19-15-49](https://user-images.githubusercontent.com/47164533/114010236-dcb47200-989e-11eb-80f1-5d293eef9a76.png)

### Create all testcases manually
```
joho-ta-tools gen -f model_answer.cpp
```

上記のコマンドの後、入力のテストケースが聞かれるので，順次入力してください。全てのテストケースを手動で入れる必要があります。

### with handmade generator
入力ケースをアウトプットするプログラム(ex. generator.cpp)を作成し、以下のコマンドを使って、10個のテストケースを生成します。

```
joho-ta-tools gen -f model_answer.cpp -g generator.cpp
```

### with special judge
TODO

## merge csv file
csvファイルは2種類必要となります。xlsxをcsvに変換する際には[dilshod/xlsx2csv](https://github.com/dilshod/xlsx2csv)のようなものが便利です。
1. ポートフォリオから抽出したファイル。以下のようなフォーマットに従っていることを確認してください。

   理工 | 1 | 5 | 1 | 安藤 | アンドウ | 153R200000
   --- | --- | --- | --- | --- | --- | ---
   理工 | 1 | 5 | 2 | 伊藤 | イトウ | 153R200001
   以下続く | | | | | |

2. online judgeから得られたコンテスト結果。またディレクトリ内に全てのコンテスト結果のcsvファイルをまとめてください。1行目を削除する必要は特にありません。以下のようなフォーマットに従っていることを確認してください。

   User ID | Username | Real Name | Total Score | Q1 | Q2 | etc
   --- | --- | --- | --- | --- | --- | ---
   123 | 153R200000 | ando | 200 | 100 | 100 | etc
   以下続く | | | | | |

### Folder Structure
2のcsvファイルは全て、同一のディレクトリに入れてください。

### arguments
- --default-csv VALUE, -d VALUE  : Value is required. 上記の1に当たるcsvファイルのパスを入力してください
- --contests-dir VALUE, -c VALUE : Value is required. 上記の2に当たるcsvファイルをまとめたディレクトリのパスを入力してください
- --output-file VALUE, -o VALUE  : output file name. default name is 'merge.csv'.

### How to use
画像のような構成の場合、以下のコマンドで結果が生成されます。

`joho-ta-tools merge -d student_list/2020_全学生リスト.csv -c contest_results -o result.csv`

![image](https://user-images.githubusercontent.com/47164533/119483430-ec751080-bd8f-11eb-9243-01cae11e0edd.png)
