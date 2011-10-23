Integrate.u是最老的基本表，暂供老版本内核使用，不再进行维护。
InTable.m是SIN-like Integration System所使用的积分表，NoCloseTable.m类似，不过内容是无闭形式被积函数表；
InTable.u和NoCloseTable.u是对应的mU内核版本。

为了保证兼容性，现在还是保存了TableInt.m和NoCloseInt.m——改自InTable.m的和Rubi3兼容的积分表。

init.m初始化，载入original目录下的Rubi3包，运行init.m之后，就可以使用Int[f,x]来查询积分表了。