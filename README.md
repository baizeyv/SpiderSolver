# CXX Spider Solver

用于尝试解决 `PlayValve` 和 `Vita` 的蜘蛛纸牌关卡的CLI小工具.

## Option
```
-h --help Show help information
-t --test Enter test mode
-d --debug Enter debug mode
-b --batch Enter batch mode
-g --generate Enter generation mode
-s --step Enter step mode
-o --output Debug mode output path, default-> ./log/***
-v --version Show spider version
```

## Mode

### TestMode
测试模式

#### Commands

> `vita [level_string]` -> Try to solve the vita level.
>
> `playvalve [seed] [suit_count]` -> Try to solve the PlayValve level.
>
> `query [vita|playvalve]` -> Query the level currently being attempted to solve.
>
> `stop [vita|playvalve]` -> Stop the level currently being attempted to solve.
>
> `view vita [vita_level_string]` -> View the Vita level cards.
>
> `view playvalve [seed] [suit_count]` -> view the PlayValve level cards.
>
> `shrink` -> Release memory.
>
> `memory` -> View memory usage.
>
> `clear` -> Clear console.
>
> `exit` -> Exit TestMode.
>
> `quit` -> Exit TestMode

### DebugMode
Debug关卡模式

#### Commands

> `vita [level_string]` -> Try to solve the vita level.
>
> `playvalve [seed] [suit_count]` -> Try to solve the PlayValve level.
>
> `stop [vita|playvalve]` -> Stop the level currently being attempted to solve.
>
> `shrink` -> Release memory.
>
> `clear` -> Clear console.
>
> `exit` -> Exit TestMode.
>
> `quit` -> Exit TestMode

### BatchMode
批量模式

#### Commands

> `vita [level_json_file_path] [output_path] ([step_limit])` -> Try to solve the Vita level. (Optional: step_limit)
>
> `playvalve [level_txt_file_path] [output_path] [suit_count] ([step_limit])` -> Try to solve the PlayValve level. (Optional: step_limit)
>
> `stop [vita|playvalve]` -> Stop the level currently being attempted to solve.
> 
> `query` -> Query Vita and PlayValve Exporter.
> 
> `shrink` -> Release memory.
>
> `clear` -> Clear console.
>
> `exit` -> Exit TestMode.
>
> `quit` -> Exit TestMode

### GenMode
生成关卡模式

#### Commands

> `generate [suit_count] [output_path] [step_limit]` -> Try to generate the level.
>
> `stop` -> Stop the generation currently being attempted to solve.
>
> `shrink` -> Release memory.
>
> `clear` -> Clear console.
>
> `exit` -> Exit TestMode.
>
> `quit` -> Exit TestMode

### StepMode
Step-By-Step模式

#### Commands

> `vita [level_string]` -> Try to solve the vita level.
>
> `playvalve [seed] [suit_count]` -> Try to solve the PlayValve level.
>
> `stop` -> Stop the currently being attempted to solve.
>
> `shrink` -> Release memory.
>
> `clear` -> Clear console.
>
> `exit` -> Exit TestMode.
>
> `quit` -> Exit TestMode

## TODO
1. 移动到空列的算法有问题，以及模拟移动中的特殊过滤器 (归根结底是同一个问题)
能不往空列放就先不放,优先移动不在空列中的
2. 优先向空列移动一整个顺子,最下牌为1的优先级降低,因为这样最多出来一个可用牌
3. sort 函数可能需要修改