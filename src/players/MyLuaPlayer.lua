--[[
Quick Guide:

There are a bunch of read-only properties you can use in your logic,
these are all information about your current bets on the table...

Player.bankRoll
Player.passBet
Player.passOdds
Player.dontPass
Player.dontPassOdds
Player.comeBet
Player.fieldBet
Player.place4
Player.place5
Player.place6
Player.place8
Player.place9
Player.place10
Player.come4
Player.come5
Player.come6
Player.come8
Player.come9
Player.come10
Player.come4Odds
Player.come5Odds
Player.come6Odds
Player.come8Odds
Player.come9Odds
Player.come10Odds
Player.hard4
Player.hard6
Player.hard8
Player.hard10
Player.any7
Player.anyCraps
Player.yoBet
Player.splitCandE

There are also some read-only properties that give you an indication
that the dealer just did something with your money, they are set
to TRUE for the one turn that they are valid:

Player.justMovedComeBet
Player.justEstablishedPoint
Player.justSevenedOut
Player.justTookDownComeBet

There are some methods you can call to get information about the
current game state...

Player:getGameState()  - returns 1 when point is off, 2 when point is on
Player:getGamePoint()  - if point is on, this will return current point
Player:getGameLastNumber()  - returns the last number that was rolled
Player:getTotalOnTable()  - returns how much you currently have out on the table

Finally, there are some methods you should call to make bets..

Player:placePassBet(amount)
Player:takePassOdds(amount)
Player:placeDontPassBet(amount)
Player:takeDontPassOdds(amount)
Player:placeComeBet(amount)
Player:takeComeOddsLastNumber(amount)
Player:placeFieldBet(amount)
Player:makePlaceBet(number, amount)
Player:makeHardwayBet(number, amount)
Player:placeAnySevenBet(amount)
Player:placeAnyCrapsBet(amount)
Player:placeYoBet(amount)
Player:placeSplitCandE(amount)

Below is an implementation of a basic player (they take pass bet, and make
up to three come bets; hey take odds on everything, and higher odds on
6 and 8).

]]--


-- define a global for us to keep track of how many bets on the table
myComeBetsThisHand = 0

-- the function that happens every turn
function play()

	-- first, lets maintain my come bet counter
	if Player.justSevenedOut then
		myComeBetsThisHand = 0
	end
	if Player.justTookDownComeBet then
		myComeBetsThisHand = myComeBetsThisHand - 1
	end
	
	-- create come local copies of game state, this
	-- helps resolve some LUA funkiness later on
	local gameState = Player:getGameState()
	local gamePoint = Player:getGamePoint()
	local lastNumber = Player:getGameLastNumber()
	local totalOnTable = Player:getTotalOnTable()
	
	-- now some basic play
	-- if the point is off, make sure we have a pass bet out
	if gameState == 1 then
		-- bet might be left over, so check first
		if Player.passBet < 5 then
			Player:placePassBet(5)
		end
	end
	
	-- some more advanced play when point is on
	if gameState == 2 then
	
		-- always take our pass odds, more odds for 6 and 8
		if Player.passOdds < 5 then
			if gamePoint == 6 or gamePoint == 8 then
				Player:takePassOdds(25)
			else
				Player:takePassOdds(10)
			end
		end
		
		-- make up to 3 come bets
		if myComeBetsThisHand < 3 then
			-- bet might already be out, so check first
			if Player.comeBet < 5 then
				myComeBetsThisHand = myComeBetsThisHand + 1
				Player:placeComeBet(5)
			end
		end
		
		-- take come odds if it was just moved to a number
		if Player.justMovedComeBet then
			-- again, bet more for 6 or 8
			if lastNumber == 6 or lastNumber == 8 then
				Player:takeComeOddsLastNumber(25)
			else
				Player:takeComeOddsLastNumber(10)
			end
		end
		
	end
end