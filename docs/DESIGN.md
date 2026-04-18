# Ashen Frontier DS – One-Level RPG Slice

## Goal
Create a **single playable DS ROM level** that feels like a condensed MMORPG/ARPG loop:
1. get quest in outpost
2. travel through a hostile field
3. fight enemies
4. loot chest
5. kill miniboss
6. return / trigger beacon exit

## Tone target
- **WoW-like**: quest giver, HUD stats, XP and gold, readable class-like combat role
- **Crimson Desert-like**: dusty frontier route, grounded action-adventure pacing, roaming danger
- **DS realistic**: 2D overhead presentation, fast loops, strong readability

## Level layout
Map is a single 32x20 field.

### Spaces
- **Outpost**: spawn point, quest giver, safe zone
- **Dust Road**: open travel lane with basic enemies
- **Broken Camp**: chest location
- **Shrine Gate**: boss zone
- **Beacon Exit**: level completion trigger after boss kill

## Core loop
- Player starts with sword, potion, and intro quest.
- Talk to Marshal NPC to accept quest.
- Clear route and gain XP.
- Open chest for potion + gold.
- Defeat miniboss.
- Exit beacon activates.
- Interact with beacon to end level.

## RPG systems in this slice
- HP / Max HP
- Attack power
- XP / level-up
- gold
- potion count
- quest state

## Entity types
- Player
- Marshal Rowan (quest giver)
- Raider mobs
- Dust Warden (miniboss)
- Chest
- Exit beacon

## Stretch ideas
- class choice at start: Knight / Hunter / Mage
- touch-screen hotbar
- inventory grid on top screen
- day/night palette swap
- boss telegraph attack pattern
