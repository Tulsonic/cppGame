#include <algorithm>
#include <array>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include "../headers/header.h"
#include "../headers/item.h"
#include "../headers/objects.h"
#include "../headers/player.h"

using namespace std;

Obj *findObj(string name) {
  for (auto obj : player.curRoom->objects) {
    if (obj->name == name) {
      return obj;
    } else {
      cout << "No chest in the room to open." << endl;
      prompt();
      return nullptr;
    }
  }
  cout << "No chest in the room to open." << endl;
  prompt();
  return nullptr;
}

void prompt() {
  string ans;
  cout << endl;
  cout << "-----------------" << endl;
  cout << "What will you do?" << endl;
  cout << "(" << player.curHealth << "/" << player.maxHealth << " HP)"
       << " : ";
  getline(cin, ans);
  ans = _toLower(ans);
  cout << endl;

  if (ans == "") {
    cout << "Invalid command, please type \"actions\" in order to view a list "
            "of possible actions."
         << endl;
    prompt();
  }

  string cmd = ans.substr(0, ans.find(' '));
  string param = ans.substr(ans.find(' ') + 1, ans.length());

  // Switch can not be used on strings so if else is the only way

  if (cmd == "actions") {
    cout << "actions - Displays all possible actions." << endl;
    cout << "look - Looks around the current area." << endl;
    cout << "check - Makes player check their status." << endl;
    cout << "use <item> - Uses an item from the inventory." << endl;
    cout << "open <obj> - Opens objects like chests, boxes, or doors." << endl;
    cout << "inspect <item> - Inspects the item." << endl;
    cout << "take <item> - Takes item and adds to inventory." << endl;
    cout << "equip <item> - Equip item form inventory." << endl;
    cout << "unequip <item> - Unequip item form inventory." << endl;
    cout << "discard <item> - Discard an item from your inventory (forever)."
         << endl;
    prompt();
  } else if (cmd == "check") {
    clearScr();
    player.check();
    prompt();

  } else if (cmd == "inspect") {
    if (param != "") {

      player.inspect(param);
      prompt();

    } else {
      cout << "What will you inspect?" << endl;
      cout << "Usage: inspect <item>" << endl;
      prompt();
    }

  } else if (cmd == "look") {

  } else if (cmd == "use") {

    if (param != "") {

      player.use(param);

      prompt();

    } else {
      cout << "What will you use?" << endl;
      cout << "Usage: use <item>" << endl;
      prompt();
    }

  } else if (cmd == "open") {
    if (param == "chest") {
      vector<Obj *> objs = player.curRoom->objects;

      Obj *o = findObj("chest");

      Chest *c = static_cast<Chest *>(o);

      c->open();

      prompt();
    }
  } else if (cmd == "take") {
    if (param != "") {

      player.takeItem(param);
      prompt();

    } else {
      cout << "What will you take?" << endl;
      cout << "usage: take <item>" << endl;
      prompt();
    }

  } else if (cmd == "equip") {
    if (param != "") {

      player.equipItem(param);
      prompt();

    } else {

      cout << "What will you equip?" << endl;
      cout << "usage: equip <item>" << endl;
      prompt();
    }
  } else if (cmd == "discard") {
    if (param != "") {
      player.discard(param);
      prompt();
    } else {
      cout << "What will you discard?" << endl;
      cout << "Usage: discard <item>" << endl;
      prompt();
    }
  } else if (cmd == "unequip") {
    if (param != "") {
      player.unequip(param);
      prompt();
    }
    {
      cout << "What will you unequip?" << endl;
      cout << "Usage: unequip <item>" << endl;
      prompt();
    }
  }

  else {
    cout << "Invalid command, please type \"actions\" in order to view a list "
            "of possible actions."
         << endl;
    prompt();
  }
}
