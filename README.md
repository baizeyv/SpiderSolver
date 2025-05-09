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

## TODO:
两种算法应该齐头并进,谁先解出来用谁的 (双线程)

求解器用法思路,
用以下的方法就不需要再搞多线程来实现了，避免很多问题可以
目前还差一个筛选过滤csv的工具,可以用c#也可以c++,c++会更快一些
1.目前有三种类型的求解器了，单色的可以一人跑一个不限次数的结果,然后找3个中的最优解
2.双色的可以跑 1000000次限制求解器,跑出来三个结果后,进行筛选合并
3.三色和四色的可以只跑两个c++的求解器,带有一定次数例如(10000000)的限制,最后对于扔没有解的再单独处理
