# CXX Spider Solver

用于尝试解决 `PlayValve` 和 `Vita` 的蜘蛛纸牌关卡的CLI小工具.

## Mode

### TestMode
测试模式

#### Commands

> `vita [level_string]` -> Try to solve the vita level.

> `playvalve [seed] [suit_count]` -> Try to solve the PlayValve level.

> `query [vita|playvalve]` -> Query the level currently being attempted to solve.

> `stop [vita|playvalve]` -> Stop the level currently being attempted to solve.

> `view vita [vita_level_string]` -> View the Vita level cards.

> `view playvalve [seed] [suit_count]` -> view the PlayValve level cards.

> `shrink` -> Release memory.

> `memory` -> View memory usage.

> `clear` -> Clear console.

> `exit` -> Exit TestMode.

> `quit` -> Exit TestMode

### DebugMode
Debug关卡模式

### BatchMode
批量模式

### GenMode
生成关卡模式

### StepMode
Step-By-Step模式