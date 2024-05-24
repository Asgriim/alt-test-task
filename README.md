# ALT TEST APP

## Dependency 
Library uses [jsoncpp](https://github.com/open-source-parsers/jsoncpp) 
and CURL. Make sure you installed it before building. 

```
sudo apt install libjsoncpp-dev libcurl4-openssl-dev
```
## Build and run
``git clone https://github.com/Asgriim/alt-test-task.git``

``cmake -B target``

``cmake --build target --target test-app``

``./target/app/test-app p10 p9 > file.json``

Where ``p10`` and ``p9`` are branch names.

## Usage 
### Cli App
Pass branch names as command line arguments.

``test-app p10 p9 > file.json``

It can be slow because of the large amount of data from ``api``. 

Recommended to redirect ``stdout``, because ``api`` response is too large.

### Library
Main class is ``Alt::BranchPackagesArranger``

```CMAKE
find_library(alttest NAMES alttest)

target_link_libraries(${TARGET_NAME} PRIVATE alttest)
```
Example

```C++

#include <alttestlib>
int main(int argc, char* argv[]) {
    const std::string apiLink = "https://rdb.altlinux.org/api/export/branch_binary_packages/";
    Alt::BranchPackagesArranger arranger(apiLink);

    //this method find exist in branch p10 and does not exist in p9
    //Result is stored in comparison field
    Alt::BranchPacksDiff uniDiff = arranger.findBranchUniqPacks("p10", "p9");

    //this method will compare packages that exist in 2 branches
    // and return branch1 packages with higher version
    Alt::BranchPacksDiff intDiff = arranger.branchesIntersection("p10", "p9");

    //You can write your own compare function like this
    Alt::BranchPacksDiff intDiffCustom = arranger
            .branchesIntersection("p10", "p9",
                   [] (const Alt::BinPackage &p1, const Alt::BinPackage &p2){
                            return p1.version < p2.version;
                        }
                   );

    //method o perform the task:
    //only_in_branch1
    //only_in_branch2
    //version_higher_in_branch1
    Alt::TargetDiff targetDiff = arranger.getTargetDiff("p10", "p9");
    return 0;
}
```

## JSON Format


```json
{
  "onlyBr1": {
    "branch1": "p10",
    "branch2": "p9",
    "branchDifference": "packages exist only in branch1",
    "comparison": {
      "aarch64": [
        {
          "arch": "aarch64",
          "buildtime": 1680082588,
          "disttag": "p10+316676.100.3.1",
          "epoch": 0,
          "name": "zuo",
          "release": "alt1",
          "source": "zuo",
          "version": "1.0.racket8.8"
        }
      ],
      "armh": [
        {
          "arch": "armh",
          "buildtime": 1680082643,
          "disttag": "p10+316676.100.3.1",
          "epoch": 0,
          "name": "zuo",
          "release": "alt1",
          "source": "zuo",
          "version": "1.0.racket8.8"
        }
      ]
    }
  },
  "onlyBr2": {
    "branch1": "p10",
    "branch2": "p9",
    "branchDifference": "packages exist only in branch2",
    "comparison": {
      "aarch64": [
        {
          "arch": "aarch64",
          "buildtime": 1561754954,
          "disttag": "p9+233404.100.1.1",
          "epoch": 0,
          "name": "zookeeper-java",
          "release": "alt5_13jpp8",
          "source": "zookeeper",
          "version": "3.4.9"
        }
      ]
    }
  },
  "verHigherBr1": {
    "branch1": "p10",
    "branch2": "p9",
    "branchDifference": "packages version higher in branch 1",
    "comparison": {
      "aarch64": [
        {
          "arch": "aarch64",
          "buildtime": 1625982015,
          "disttag": "sisyphus+278032.100.1.2",
          "epoch": 0,
          "name": "zziplib-utils-debuginfo",
          "release": "alt1",
          "source": "zziplib",
          "version": "0.13.72"
        }
      ]
    }
  }
}
```