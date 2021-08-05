# Logger Class in Singleton

### Description
* 다른 클래스에서 참조해 손쉽게 로그를 기록하기 위해 만든 클래스
* 한 달 주기로 각 날짜별로 파일을 생성하고 달이 넘어가면 해당 날짜에 해당하는 파일을 삭제하고 다시 생성한다.
* 실행 중에 외부에서 로그 레벨을 변경하는 함수가 호출되면 재컴파일 하지 않고도 로그 레벨을 변경해서 기록한다.
* 싱글턴으로 작성하였기 때문에, 한 번 로그레벨이 변경되면 동일한 인스턴스가 계속 사용되므로 동작에 안정성이 있다. 

### Environment
* OS : Ubuntu 18.04
* gcc version 7.5.0

### Files
* Logger.cpp: 다음과 같이 나뉜다
  * 파일 처리
  * 파일에 남기는 기준 로그 레벨을 외부에서 조작
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
