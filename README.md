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
g++ joho-ta-tools.cpp -o joho-ta-tools
echo "PATH=\"$PATH:~/.joho-ta-tools\"" >> ~/.bashrc
source ~/.bashrc
```

## generate testcase
testcase作成の自動化ツール。模範解答(.c/.cpp)は手元にあること前提でそこからテストケースを半自動作成します。randomモードが一番使い勝手がいいと思います。

### Folder Structure
基本のフォルダ構成は以下のように行う。
![Screenshot from 2021-04-08 19-10-12](https://user-images.githubusercontent.com/47164533/114009434-19339e00-989e-11eb-8fa6-900e4ce0ea4f.png)


### How to use
![Screenshot from 2021-04-08 19-12-53](https://user-images.githubusercontent.com/47164533/114009799-72033680-989e-11eb-9db0-66de39ebdd8e.png)
`joho-ta-tools gen -r num -f hoge.cpp`で`hoge.cpp`に対するテストケースがnum個できる。
`Input format:`に続いて、入力の型を決定する。入力の際のルールは以下に従う。
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

最終的なフォルダ構成は以下のようになる。`zips/hoge.zip`をojの`Testcase choose file`というところに入れる。
![Screenshot from 2021-04-08 19-15-49](https://user-images.githubusercontent.com/47164533/114010236-dcb47200-989e-11eb-80f1-5d293eef9a76.png)


### with special judge
TODO

## merge csv file
csvファイルは2種類必要となります。
1. ポートフォリオから抽出したファイル。以下のようなフォーマットに従っていることを確認してください。

   理工 | 1 | 5 | 1 | 安藤 | アンドウ | 153R200000
   --- | --- | --- | --- | --- | --- | ---
   理工 | 1 | 5 | 2 | 伊藤 | イトウ | 153R200001
   以下続く | | | | | |

2. online judgeから得られたコンテスト結果。.xlsxを.csvに変換してください。またディレクトリ内に全てのコンテスト結果のcsvファイルをまとめてください。1行目を削除する必要は特にありません。以下のようなフォーマットに従っていることを確認してください。

   User ID | Username | Real Name | Total Score | Q1 | Q2 | etc
   --- | --- | --- | --- | --- | --- | ---
   123 | 153R200000 | ando | 200 | 100 | 100 | etc
   以下続く | | | | | |

### ファイル構成
1のcsvファイルをone.csv、2のcsvファイルをtwo1.csv, two2.csv...、2のcsvファイルをまとめて入れるディレクトリをcontest_resultsとすると、

```
one.csv
contest_results--two1.csv
               |-two2.csv
               |-etc
```

という構成にしてください。

### How to use
上のようなファイル構成を用意できればone.csvのあるディレクトリ上で以下のコマンドを実行してください。

`joho-ta-tools merge -d one.csv -c contest_results -o output.csv`



## 注意
- random modeで、文字範囲の入力はASCIIコード表に準ずるため、A~zを指定した時、`[, \, ]`などの記号も含まれる。小文字のみか大文字のみを指定することを推奨。
- 色々バグがあると思うんで、バグの再現例とissueかpull reqお願いします。
