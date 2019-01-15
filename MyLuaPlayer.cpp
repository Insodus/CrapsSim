#include "StdAfx.h"
#include "MyLuaPlayer.h"

MyLuaPlayer::MyLuaPlayer(void)
{
	// init player name
	setPlayerName("MyLuaPlayer");

	try
	{
		// init lua objects
		myLuaState = luaL_newstate();
		luaL_openlibs(myLuaState);
		luabind::open(myLuaState);

		// define player object for our scripts
		luabind::module(myLuaState) [
			luabind::class_<MyLuaPlayer>("MyLuaPlayer")
				.def_readonly("bankRoll", &Player::bankRoll)
				.def_readonly("passBet", &Player::passBet)
				.def_readonly("passOdds", &Player::passOdds)
				.def_readonly("dontPass", &Player::dontPass)
				.def_readonly("dontPassOdds", &Player::dontPassOdds)
				.def_readonly("comeBet", &Player::comeBet)
				.def_readonly("fieldBet", &Player::fieldBet)
				.def_readonly("place4", &Player::place4)
				.def_readonly("place5", &Player::place5)
				.def_readonly("place6", &Player::place6)
				.def_readonly("place8", &Player::place8)
				.def_readonly("place9", &Player::place9)
				.def_readonly("place10", &Player::place10)
				.def_readonly("hard4", &Player::hard4)
				.def_readonly("hard6", &Player::hard6)
				.def_readonly("hard8", &Player::hard8)
				.def_readonly("hard10", &Player::hard10)
				.def_readonly("any7", &Player::any7)
				.def_readonly("anyCraps", &Player::anyCraps)
				.def_readonly("yoBet", &Player::yoBet)
				.def_readonly("splitCandE", &Player::splitCandE)
				.def_readonly("come4", &Player::come4)
				.def_readonly("come5", &Player::come5)
				.def_readonly("come6", &Player::come6)
				.def_readonly("come8", &Player::come8)
				.def_readonly("come9", &Player::come9)
				.def_readonly("come10", &Player::come10)
				.def_readonly("come4Odds", &Player::come4Odds)
				.def_readonly("come5Odds", &Player::come5Odds)
				.def_readonly("come6Odds", &Player::come6Odds)
				.def_readonly("come8Odds", &Player::come8Odds)
				.def_readonly("come9Odds", &Player::come9Odds)
				.def_readonly("come10Odds", &Player::come10Odds)
				.def_readonly("justMovedComeBet", &Player::justMovedComeBet)
				.def_readonly("justEstablishedPoint", &Player::justEstablishedPoint)
				.def_readonly("justSevenedOut", &Player::justSevenedOut)
				.def_readonly("justTookDownComeBet", &Player::justTookDownComeBet)
				.def("placePassBet", &Player::placePassBet)
				.def("takePassOdds", &Player::takePassOdds)
				.def("placeDontPassBet", &Player::placeDontPassBet)
				.def("takeDontPassOdds", &Player::takeDontPassOdds)
				.def("placeComeBet", &Player::placeComeBet)
				.def("takeComeOddsLastNumber", &Player::takeComeOddsLastNumber)
				.def("placeFieldBet", &Player::placeFieldBet)
				.def("makePlaceBet", &Player::makePlaceBet)
				.def("makeHardwayBet", &Player::makeHardwayBet)
				.def("placeAnySevenBet", &Player::placeAnySevenBet)
				.def("placeAnyCrapsBet", &Player::placeAnyCrapsBet)
				.def("placeYoBet", &Player::placeYoBet)
				.def("placeSplitCandE", &Player::placeSplitCandE)
				.def("getGameState", &Player::getGameState)
				.def("getGamePoint", &Player::getGamePoint)
				.def("getGameLastNumber", &Player::getGameLastNumber)
				.def("getTotalOnTable", &Player::getTotalOnTable)
		];

		// pass our player down into lua
		luabind::globals(myLuaState)["Player"] = this;
	}
	catch (std::exception &theExc)
	{
		std::cerr << theExc.what() << std::endl;
		exit(1);
	}
}

MyLuaPlayer::~MyLuaPlayer(void)
{
	delete luaEntityTable;
	lua_close(myLuaState);
}

void MyLuaPlayer::setScriptName(std::string scriptName)
{
	this->scriptName = scriptName;
	int luaRet = luaL_dofile(myLuaState, scriptName.c_str());
	if (luaRet)
	{
		std::cerr << "LUA Error!" << std::endl;
		std::cerr << "Error Message: " << lua_tostring(myLuaState, -1) << std::endl;
		exit(1);
	}
}

void MyLuaPlayer::play()
{
	try
	{
		luabind::call_function<void>(myLuaState, "play");
	}
	catch (const luabind::error &theExc)
	{
		std::cerr << "LUA Error!" << std::endl;
		std::cerr << "Error Message: " << theExc.what() << std::endl;
		exit(1);
	}
}

