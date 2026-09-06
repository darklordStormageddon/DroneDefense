# 🛸 Drone Defense (드론 디펜스)

> **Unreal Engine 5 기반 3인칭 액션 디펜스 게임**  
> 사방에서 몰려오는 몬스터들로부터 중앙의 넥서스를 지키고, 최종 보스를 격파하세요!

<br/>

## 📌 1. 프로젝트 개요 (Overview)

- **개발 기간:** 4주
- **엔진/언어:** Unreal Engine 5.3, C++ / Blueprint
- **장르:** 3인칭 액션 디펜스 (3P Action Defense)
- **주요 특징:**
  - `G` 키를 활용한 3가지 **드론 모드(요격/추적/분쇄)** 실시간 체인지 메커니즘
  - 4방향 다중 스폰 및 5개 **웨이브(Wave) 진행 시스템**
  - 보스 등장 시 전체 몬스터 **버프 부여 스킬** 구현

<br/>

## 👥 2. 팀원 및 역할 (Team & Roles)

| 프로필 | 이름 | 담당 역할 | 주요 구현 내용 |
| :---: | :---: | :---: | :--- |
| 👑 | **김상민** | **팀장** / AI & 전투 | • 몬스터(보스, 미니언) AI 및 이동 로직 개발<br>• 플레이어 ↔ 몬스터 전투 상호작용 (공격/피격/스킬)<br>• 넥서스 체력 및 상호작용 시스템<br>• 실시간 UI 미니맵 제작 |
| 🛸 | **조현수** | 드론 & 메커니즘 | • 드론 3종 메커니즘 구현 (요격/추적/분쇄)<br>• 전체 게임 플로우 설계<br>• Git/GitHub 협업 환경 구축 및 버전 관리 |
| 🎯 | **최주현** | 스폰 & 밸런싱 | • 웨이브(Wave) 시스템 구축<br>• 스탯 기반 몬스터 소환 및 난이도 밸런싱<br>• 플레이어 사격 모드 디자인 및 메커니즘 구현 |
| 💻 | **이형주** | 인게임 UI | • 체력바(HP Bar) 및 웨이브 UI 개발<br>• 게임 상태 UI (Wave Clear / Game Clear / Game Over) 구현 |
| 🎨 | **용승호** | 레벨 디자인 & UI | • 맵 레벨 디자인 구축<br>• 메인 시스템 UI (타이틀 / 로딩 / Pause 메뉴)<br>• 보스 등장 시네마틱 컷씬 연출 및 파티클 이펙트 조정 |

<br/>

## 🎮 3. 게임 플레이 및 조작법 (How to Play)

### 🕹️ 조작키 (Controls)
- `마우스 좌클릭`: 플레이어 기본 총기 사격
- `G`: 드론 프리셋(모드) 변경
- `P`: 게임 일시정지 (Resume / Go Title / Quit)

---

### 🛸 드론 모드 (Drone Presets)
- **Preset 1 (요격):** 투사체 발사형 방어 및 지원 사격
- **Preset 2 (추적):** 다수의 드론이 적을 추적하여 분산 공격
- **Preset 3 (분쇄):** 근접 회전 공격으로 주변 적 밀집 타격

---

### ⚔️ 주요 시스템 (Core Systems)
[4방향 스폰] ──> [몬스터 이동] ──> [넥서스 방어 (10,000 HP)] ->

[WARNING: 보스 스킬 (전체 버프)] -> [보스 처치] -> [Wave 5 클리어]

1. **넥서스 방어:** 맵 중앙에 위치한 넥서스(10,000 HP)가 파괴되지 않도록 방어해야 합니다.
2. **웨이브 시스템:** 총 5개의 웨이브로 구성되어 있으며, 시작/종료 웨이브 설정이 가능합니다.
3. **미니맵 지원:** 사방의 스폰 위치, 플레이어, 넥서스, 적의 동선을 한눈에 파악할 수 있습니다.
4. **보스 이벤트 (Wave 5):** 
   - 5웨이브의 모든 미니언 소환 완료 후 **보스 몬스터**가 등장합니다.
   - 보스의 스킬 발동 시, 경고 화면(`WARNING`)과 함께 **맵 내 전체 몬스터의 능력치가 일시적으로 상승**합니다.

<br/>

## 🖼️ 4. 게임 화면 (Screenshots)

**드론을 이용한 공격**
<img width="1041" height="581" alt="image" src="https://github.com/user-attachments/assets/206c96ac-41bf-4995-8b29-ca79435db175" />

**보스 몬스터 스킬 사용 시 다른 몬스터 능력치 상승**
<img width="816" height="459" alt="image" src="https://github.com/user-attachments/assets/ab1b0cb0-617b-41db-8d17-5b83d212d9fe" />


---

<br/>

## 🎬 5. 시연 영상 (Demo Video)

[![드론디펜스 시연 영상](https://img.youtube.com/vi/Yx5WeOaiU18/0.jpg)](https://youtu.be/Yx5WeOaiU18)
* 🔗 **YouTube 링크**: [Drone Defense 시연 영상 보러가기]([https://youtu.be/Yx5WeOaiU18])

---

<br/>

## 🔍 6. 추가 연구 (렌더링 최적화) (Further Research)

📋[Drone Defense 렌더링 최적화 보고서.pptx](https://github.com/user-attachments/files/31877948/Drone.Defense.pptx)

<br/>


