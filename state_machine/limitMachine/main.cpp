//独立的有限状态机
//该状态机只有两个状态,每个状态相互独立,且没有状态转移
STATE_MACHINE(Packege _pack)
{
  PackegeType _type = _pack.GetType(); 
  swich(_type)
  {
    case type_A:
      process_packege_A(_pack);
      break;
    case type_B:
      process_packege_B(_pack);
      break;
  }
}
