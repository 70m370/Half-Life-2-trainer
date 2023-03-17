//the idea here is to first create a simple trainer, Ammo, Life and stuff i found in code, like player movement whatever
// 
//our trainer will create a dll, and we are gonna hardcode a injector later, but first we'll use GHinjector
// 
//Roadmap

// I cant copy code.
// 
//Learn and use Directx features, maybe creating my first esp and stuff
//do it internal, learn about dll, memory in general, i already know the basics
// Reverse structures, source code here :> https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/server/player_command.cpp
// use ida to go on with addresses and rename stuff based on the source code above
// 
//hack assembly
//
//First find the information we are gonna change, first of all it will be only life and ammo.
//
//Then create a DLL with the procedure that will attach to the process
//
//test it using the GHinjector, then create my own injector.



//hack info so far - addresses in the cheattable

//i got the player life addres, arthur helped- me
//to have infinity ammo, I'll nope some bytes