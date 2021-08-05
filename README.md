# Logger Class in Singleton

### Description
* 다른 클래스에서 참조해 손쉽게 로그를 기록하기 위해 만든 클래스
* 한 달 주기로 각 날짜별로 파일을 생성하고 달이 넘어가면 해당 날짜에 해당하는 파일을 삭제하고 다시 생성한다.

### Environment
* OS : Ubuntu 18.04
* gcc version 7.5.0

### Prerequisite
* .

### Files
* Logger.cpp: 다음과 같이 나뉜다
  * 파일 처리
  * 로그 레벨을 외부에서 조작
  * 로그 기록
* Logger.h: 매크로 및 함수 정의

### Usage
* 클래스 헤더에서 다음과 같이 선언한 뒤 매크로를 통해 로그 함수를 호출한다.
```
//GitHub.h
class GitHub {
private:
    Logger *p_Logger = Logger::getInstance();       // Logger instance 호출
public:
    void GitHubFunc();
}

//GitHub.cpp
void GitHub::GitHubFunc() {
    p_Logger->INFO("GitHubFunc Executed", 1);
}

```
