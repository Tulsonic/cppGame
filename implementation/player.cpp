#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <vector>

using namespace std;

#include "../headers/header.h"
#include "../headers/item.h"
#include "../headers/objects.h"
#include "../headers/player.h"
#include "../headers/rooms.h"

Player::Player(int _health, vector<unique_ptr<Item>> _inv,
               array<unique_ptr<Item>, 4> _clothes, unique_ptr<Item> _eqp)
    : maxHealth(_health), curHealth(_health) {
  clothes = move(_clothes);
  inv = move(_inv);
  eqp = move(_eqp);
  calcDef();
};

unique_ptr<Item> Player::findItemInv(string name) {
  for (auto & i : inv) {
    if (_toLower(i->name) == name) {
      return move(i);
    }
  }
  return 0;
}

void Player::calcDef() {
  def = 0;
  for (auto &c : clothes) {
    if (c != nullptr) {
      def += c->prop;
    }
  }
}

void Player::removeFromInv(unique_ptr<Item> &i) {
  inv.erase(find(inv.begin(), inv.end(), i));
}

void Player::moveTo(shared_ptr<Room> room) { 
  curRoom = room;
  room->init();
}

void Player::takeDamage(int dmg) {
  // hurt the player
  curHealth -= dmg;
}

void Player::use(string name) {
  unique_ptr<Item> i = findItemInv(name);

  if (i == nullptr) {
    cout << "Item not present in inventory." << endl;
    prompt();
  }

  switch (i->type) {
  case 0:
    // puzzle
    break;
  case 1:
    // consumable
    if (curHealth + i->prop > maxHealth) {
      curHealth = maxHealth;
    } else {
      curHealth += i->prop;
    }

    cout << i->dia << endl;

    if (i->prop > 0) {
      cout << "You gained +" << i->prop << " HP" << endl;
    } else {
      cout << "You lost -" << i->prop << " HP" << endl;
    }

    removeFromInv(i);

    break;
  default:
    cout << "Cannot use a " << i->name << endl;
    break;
  }
}

void Player::takeItem(string name) {
  // add item to inventory

  vector<unique_ptr<Item>> &rItems = curRoom->items;
  unique_ptr<Item> i;

  if (rItems.size() == 0) {
    cout << "Item not present in the room." << endl;
    prompt();
  }

  for (auto &item : rItems) {
    if (_toLower(item->name) == name) {
      i = move(item);
      break;
    }
  }

  if (i == nullptr) {
    cout << "Item not present in the room." << endl;
    prompt();
  }

  cout << "You took \"" << i->name << "\"." << endl;

  inv.push_back(move(i));
  rItems.erase(find(rItems.begin(), rItems.end(), i));
}

void Player::equipItem(string name) {
  // Check if the item that is being equiped is within the inventory

  unique_ptr<Item> i = findItemInv(name);


  if (i == nullptr) {
    cout << "Item not present in inventory." << endl;
    prompt();
  } else if (i->type == 2) {

    // weapon
    removeFromInv(i);

    if (eqp != nullptr) {
      inv.push_back(move(eqp));
    }

    cout << "You equipped \"" << i->name << "\"." << endl;

    eqp = move(i);

  } else if (i->type <= 6 && i->type >= 2) {
    // chlothing
    
    removeFromInv(i);

    if (clothes[i->type - 3] != nullptr) {
      inv.push_back(move(clothes[i->type - 3]));
    }

    cout << "You equipped \"" << i->name << "\"." << endl;

    clothes[i->type - 3] = move(i);

    calcDef();
  }

  else {
    cout << "This item can not be equipped." << endl;
  }

}

void Player::check() {
  cout << "|--------Status---------|" << endl;
  cout << "   -  Health: "
       << "\t" << curHealth << "/" << maxHealth << " HP" << endl;
  cout << "   -  Defense: "
       << "\t" << def << " DP" << endl;
  cout << "|-------Inventory-------|" << endl;
  for (auto &item : inv) {
    cout << "   -  " << item->name << endl;
  }
  cout << "|--------Clothes--------|" << endl;
  for (auto &item : clothes) {
    if (item == nullptr) {
      cout << "   -  Empty" << endl;
    } else {
      cout << "   -  " << item->name << endl;
    }
  }
  cout << "|-------Equipped--------|" << endl;
  if (eqp == nullptr) {
    cout << "   -  Empty" << endl;
  } else {
    cout << "   -  " << eqp->name << endl;
  }
  cout << "|-----------------------|" << endl;
}

void Player::inspect(string name) {
  unique_ptr<Item> i = findItemInv(name);

  if (i == nullptr) {
    cout << "Item not present in inventory." << endl;
    prompt();
  }

  cout << i->desc << endl;

  return;
}

void Player::discard(string name) {
  unique_ptr<Item> i = findItemInv(name);

  if (i == nullptr) {
    cout << "Item not present in inventory." << endl;
    prompt();
  }

  if (i->type != 0) {
    removeFromInv(i);

    cout << "Discarded \"" << i->name << "\"." << endl;

    i.reset();
  } else {
    cout << "This item seems important." << endl;
  }
}

void Player::unequip(string name) {

  unique_ptr<Item> c = nullptr;

  for (auto &_c : clothes) {
    if (_c != nullptr && _toLower(_c->name) == name) {
      c = move(_c);
    }
  }

  if (c != nullptr) {
    cout << "You took off " << c->name << "." << endl;
    inv.push_back(move(c));
    calcDef();
  } else if (eqp != nullptr && _toLower(eqp->name) == name) {
    cout << eqp->name << endl;
    cout << "You put " << eqp->name << " back into your bag." << endl;
    inv.push_back(move(eqp));
  } else {
    cout << "No item to remove." << endl;
  }
}

Player player(50, {}, {}, make_unique<Item>(0));
