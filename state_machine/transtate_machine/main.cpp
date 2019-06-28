//带状态转移的有限状态机
//该状态机有三种状态,状态转移靠内部驱动
//状态A是状态机的开始状态,状态C是结束状态
//开始时,_curState是状态A执行完A对应的动作后,通过赋值改变_curState
//使得状态改变,下一次循环到来时执行状态B的动作,依次循环直到结束
STATE_MACHINE()
{
  state _curState = type_A;
  while(_curState != type_C)
  {
    Packege _pack = GetNewPackege();
    switch(_curState)
    {
      case type_A:
        process_state_type_A(_pack);
        _curState = type_B;
        break;
      case type_B:
        process_state_type_B(_pack);
        _curState = type_C;
        break;
    }
  }
}
