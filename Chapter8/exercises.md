#5. 다음의 쟁점들을 참고하여 연속적 메모리 할당, 순수 세그먼테이션, 순수 페이징 기법들을 비교하시오.

  - 외부 단편화
    * 연속적 메모리 할당 :
        가용 공간이 이곳저곳 산재할 가능성이 있다.
    * 순수 세그먼테이션 : 
        각 세그먼트들이 다른 크기를 가지고 있으므로 발생할 가능성이 있다.
    * 순수 페이징 : 
        물리 및 논리 메모리를 단위화해놓았으므로 발생하지 않는다.

  - 내부 단편화
    * 연속적 메모리 할당 :
        할당된 가용 공간의 상태에 따라 크게 발생할 수도 있다.
    * 순수 세그먼테이션 : 
        한계 및 기준 레지스터를 조정하여 해결이 가능하다.
    * 순수 페이징 : 
        최악의 경우 한 페이지에 할당된 프레임이 한 프레임를 제외하고 전부 사용 불가능하게 할당되는 경우가 발생할 수 있다.

  - 프로세스 사이의 코드 공유 가능성
    * 연속적 메모리 할당 :
        힘들다. 코드의 공유를 위해 해당 부분의 시작 주소와 끝 주소를 별도로 저장해놓아야 하며,
        공유 코드 실행 후 돌아갈 루틴의 주소도 저장해야 하므로 비효율적이며 위험하다.
    * 순수 세그먼테이션 :
        세그먼트 테이블에 공유 코드를 등록해놓으면 된다.
    * 순수 페이징 : 
        한 프레임에 여러 가상 주소를 할당함으로서 쉬운 공유가 가능하다.

#13. BTV 운영체제는 21-비트 가상 주소를 사용하지만, 특정 임베디드 장치의 경우 물리 메모리 주소로 16-비트를
#   사용한다. 다음 테이블의 항목의 개수는 몇 개인가?

    a. 전통적인 1단계 페이지 테이블
        <pageNumber, offset> -> <frameNumber, offset>
        [p d] -> [f d]
        오프셋의 값이 일정하기 떄문에, pd/fd = 2^5이고, 따라서 d=5이다. 그러므로 p=16, f=11이며 페이지 테이블 항목 수는 16이다.

    b. 역 페이지 테이블
        <PID, pageAddr, offset> -> <pageTableIndex, offset>
        [pid p d] -> [i d]
        역 페이지 테이블은 메모리 프레임마다 한 항목을 할당하는데, 따라서 페이지 테이블 항목 수는 11이다.

#17. 메모리에 페이지 테이블을 저장하는 페이징 시스템을 생각해 보자.
    a. 만약 메모리 참조가 50 nsec 걸린다면 메모리 참조는 얼마나 걸리는가?
        x * 2 * 50nsec = x번 참조 시 100nsec
    b. TLB를 추가하여 모든 페이지 테이블 참조의 75%를 TLB에서 찾을 수 있다면 실제 메모리 접근 시간은 얼마인가?
        (TLB에 존재하는 페이지 테이블 항목을 찾는 데 2nsec)
        0.25x * (2 * 50nsec) + 0.75x * (2+50nsec) = x (25 + 52*0.75) = x(25+ 39) = x * (64 nsec)

#18. 세그멘테이션과 페이징 기법을 결합하여 사용하는 이유는 무엇 때문인가?
    세그먼테이션은 메모리 영역을 추상화시키고, 페이징은 메모리 영역을 단위화해 분할한다.
    페이징으로 메모리를 프레임으로 쪼개 외부 단편화를 해결하고,
    세그멘테이션으로 프로그램을 쪼개 기준과 한계를 설정해 내부 단편화를 완화할 수 있다.
