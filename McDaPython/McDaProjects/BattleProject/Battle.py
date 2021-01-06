# This is the real code for my battling (by Seth McDonald)

import pygame
from random import randint, uniform
from time import time

pygame.init()
window = pygame.display.set_mode((500, 500))

pygame.display.set_caption("Python Battle Simulator")
icon = pygame.image.load("BattleProject/Images/GameIcon.ico")
pygame.display.set_icon(icon)


class User:
    def __init__(self, level, no_of_opp, name, screen):
        self.level = level
        self.noOfOpp = no_of_opp
        self.health = 40 * level
        self.healthMeas = 40 * level
        self.opp1Hit = False
        self.opp2Hit = False
        self.opp3Hit = False
        self.battleInput = 1
        self.choiceOfOpp = 1
        self.magicInput = 0
        self.itemInput = 0
        self.back = False
        self.attackDamage = 0
        self.attackUse = False2
        self.magicUse = False
        self.defIncUse = False
        self.atkIncUse = False
        self.tauntUse = False
        self.tauntAmount = 1
        self.magicDefence = 1
        self.magicAttack = 1
        self.magicEffect = 1
        self.defenceIncrease = 1
        self.attackIncrease = 1
        self.healthIncrease = 0
        self.healthOverStart = 0
        self.useReflector = False
        self.reflected = False
        self.reflectorHealth = 100
        self.absorberHealth = 100
        self.reflectorRandom = 0
        self.absorberRandom = 0
        self.colour = (0, 255, 0)
        self.window = screen
        self.name = name

    def health_bar(self):
        font = pygame.font.SysFont("arial", 40)
        text = font.render(self.name, True, (0, 0, 0))
        self.window.blit(text, (275, 0))

        if self.health > self.healthMeas * 0.5:
            self.colour = (0, 255, 0)

        elif self.health > self.healthMeas * 0.2:
            self.colour = (255, 165, 0)

        else:
            self.colour = (255, 0, 0)

        pygame.draw.rect(self.window, (0, 0, 0), (275, 50, 200, 50), 0)
        pygame.draw.rect(self.window, self.colour, (275, 50, (self.health / self.healthMeas) * 200, 50))

    def reflector(self, opp_no, opp_attack):
        self.reflectorRandom = randint(1, 100)

        if self.reflectorHealth < 0:
            if opp_attack == 0:
                return f"But your reflector had already been destroyed\nOpponent {opp_no}'s attack missed!"

            else:
                if opp_attack > self.health:
                    opp_attack = self.health

                self.health -= opp_attack
                return f"But your reflector had already been destroyed\nOpponent {opp_no} did {opp_attack} damage!"

        elif self.reflectorHealth == 0:
            if opp_attack == 0:
                return f"Opponent {opp_no}'s attack missed!"

            else:
                opp_attack *= 1.6
                opp_attack = int(opp_attack)

                if opp_attack > self.health:
                    opp_attack = self.health

                self.health -= opp_attack
                self.reflectorHealth -= 20
                return f"Opponent {opp_no}'s attack destroyed your reflector!" \
                       f"Opponent {opp_no} did {opp_attack} damage!"

        elif self.reflectorRandom > self.reflectorHealth:
            if opp_attack == 0:
                return f"Opponent {opp_no}'s attack missed!"

            else:
                opp_attack *= 1.3
                opp_attack = int(opp_attack)

                if opp_attack > self.health:
                    opp_attack = self.health

                self.health -= opp_attack
                return f"Opponent {opp_no}'s attack broke through your reflector!" \
                       f"Opponent {opp_no} did {opp_attack} damage!"

        else:
            if opp_attack == 0:
                return f"Opponent {opp_no}'s attack missed!"

            else:
                self.reflected = True
                self.reflectorHealth -= 20
                return f"Opponent {opp_no}'s attack was reflected!" \
                       f"Opponent {opp_no}'s attack reflected with {opp_attack} damage"

    def absorber(self, opp_no, opp_attack):
        self.absorberRandom = randint(1, 99)

        if self.absorberHealth < 0:
            if opp_attack == 0:
                return f"But your absorber had already been destroyed\nOpponent {opp_no}'s attack missed!"

            else:
                if opp_attack > self.health:
                    opp_attack = self.health

                self.health -= opp_attack
                return f"But your absorber had already been destroyed\nOpponent {opp_no} did {opp_attack} damage!"

        elif self.absorberHealth == 0:
            if opp_attack == 0:
                return f"Opponent {opp_no}'s attack missed!"

            else:
                opp_attack *= 1.6
                opp_attack = int(opp_attack)

                if opp_attack > self.health:
                    opp_attack = self.health

                self.health -= opp_attack
                self.absorberHealth -= 20
                return f"Opponent {opp_no}'s attack destroyed your absorber!" \
                       f"Opponent {opp_no} did {opp_attack} damage!"

        elif self.absorberRandom > self.absorberHealth:
            if opp_attack == 0:
                return f"Opponent {opp_no} attack missed!"

            else:
                opp_attack *= 1.3
                opp_attack = int(opp_attack)

                if opp_attack > self.health:
                    opp_attack = self.health

                self.health -= opp_attack
                return f"Opponent {opp_no}'s attack broke through your absorber!" \
                       f"Opponent {opp_no} did {opp_attack} damage!"

        else:
            if opp_attack == 0:
                return f"Opponent {opp_no}'s attack missed!"

            else:
                opp_attack /= 200

                if opp_attack < 1:
                    opp_attack += 1

                self.magicEffect *= opp_attack
                self.magicEffect = int(self.magicEffect)
                self.absorberHealth -= 20
                return f"Opponent {opp_no}'s attack was absorbed!" \
                       f"Your magic power increased by {self.magicEffect * 100 - 100}%"

    def magic_attack(self):
        self.attackIncrease = uniform(1.1, 1.6) * self.magicEffect
        self.magicAttack *= self.attackIncrease
        self.attackIncrease = int(self.attackIncrease * 100 - 100)
        return f"You used magic on your attack\nYour attack increased by {self.attackIncrease}%!"

    def magic_defence(self):
        self.defenceIncrease = uniform(0.5, 1) / self.magicEffect
        self.magicDefence *= self.defenceIncrease
        self.defenceIncrease = int(100 - self.defenceIncrease * 100)
        return f"You used magic on your defence\nYour defence increased by {self.defenceIncrease}%!"

    def magic_health(self):
        self.healthIncrease = randint(10, 41) * self.magicEffect * self.level / 5
        self.health += self.healthIncrease

        if self.health > self.healthMeas:
            self.healthIncrease -= self.health - self.healthMeas
            self.health = self.healthMeas

        return f"You used magic on your health\nYour health increased by {self.healthIncrease}!"

    def taunt(self):
        self.taunt += uniform(0.20, 0.71)

        if self.noOfOpp == 1:
            return "You taunted the opponent\nThe opponent got angry!" \
                   "The opponent's attack increased\nThe opponent's defence decreased"

        else:
            return "You taunted the opponents\nThe opponents got angry!" \
                   "The opponents' attack increased\nThe opponents' defence decreased"

    def attack(self, opp_taunt, opp_no):
        global passed

        self.attackUse = True
        self.attackDamage = self.magicAttack * self.tauntAmount * opp_taunt * (self.level / 5)

        if passed <= 0.001:
            self.attackDamage = 0
        elif passed <= 0.1:
            self.attackDamage *= 60
        elif passed <= 0.15:
            self.attackDamage *= 58
        elif passed <= 0.2:
            self.attackDamage *= 56
        elif passed <= 0.25:
            self.attackDamage *= 54
        elif passed <= 0.3:
            self.attackDamage *= 52
        elif passed <= 0.35:
            self.attackDamage *= 50
        elif passed <= 0.4:
            self.attackDamage *= 48
        elif passed <= 0.45:
            self.attackDamage *= 46
        elif passed <= 0.5:
            self.attackDamage *= 44
        elif passed <= 0.55:
            self.attackDamage *= 42
        elif passed <= 0.6:
            self.attackDamage *= 40
        elif passed <= 0.65:
            self.attackDamage *= 38
        elif passed <= 0.7:
            self.attackDamage *= 36
        elif passed <= 0.75:
            self.attackDamage *= 34
        elif passed <= 0.8:
            self.attackDamage *= 32
        elif passed <= 0.85:
            self.attackDamage *= 30
        elif passed <= 0.9:
            self.attackDamage *= 28
        elif passed <= 0.95:
            self.attackDamage *= 26
        elif passed <= 1:
            self.attackDamage *= 24
        elif passed <= 1.05:
            self.attackDamage *= 22
        elif passed <= 1.1:
            self.attackDamage *= 20
        elif passed <= 1.15:
            self.attackDamage *= 18
        elif passed <= 1.2:
            self.attackDamage *= 16
        elif passed <= 1.25:
            self.attackDamage *= 14
        elif passed <= 1.3:
            self.attackDamage *= 12
        elif passed <= 1.35:
            self.attackDamage *= 10
        elif passed <= 1.4:
            self.attackDamage *= 8
        elif passed <= 1.45:
            self.attackDamage *= 6
        elif passed <= 1.5:
            self.attackDamage *= 4
        elif passed <= 1.6:
            self.attackDamage *= 2
        else:
            self.attackDamage = 0

        self.attackDamage = int(self.attackDamage)

        if self.attackDamage == 0:
            return f"You attacked Opponent {opp_no}\nYour attack missed!!"
        else:
            return f"You attacked Opponent {opp_no}\nYou did {self.attackDamage}!"


class Enemy:
    def __init__(self, screen, x, y, difficulty):
        self.enemyImg = pygame.image.load("Images/EnemyImg.ico")
        self.enemyImgX = x
        self.enemyImgY = y
        self.window = screen
        self.difficulty = difficulty
        self.health = 60 * difficulty
        self.healthMeas = 60 * difficulty
        self.attack = 0
        self.action = 1
        self.choseAction = 1  # 1 - Reflector, 2 - Absorber, 3 - Up Attack, 4 - Up Defence, 5 - Up Health, 6 - Taunt,
        # 7 - Attack
        self.taunt = 1
        self.tauntIncrease = 0
        self.magic = 1
        self.magicDefence = 1
        self.magicAttack = 1
        self.magicEffect = 1
        self.defenceIncrease = 1
        self.attackIncrease = 1
        self.healthIncrease = 0
        self.defUnder50 = 0
        self.defOver50 = 0
        self.item = 1
        self.reflectorHealth = 100
        self.absorberHealth = 100
        self.reflectorRandom = 1
        self.absorberRandom = 1
        self.reflected = False
        self.colour = (0, 255, 0)

    def draw(self):
        self.window.blit(self.enemyImg, (self.enemyImgX, self.enemyImgY))

    def health_bar(self):
        font = pygame.font.SysFont("arial", 40)
        text = font.render("Opp", True, (0, 0, 0))
        self.window.blit(text, (25, 0))

        if self.health > self.healthMeas * 0.5:
            self.colour = (0, 255, 0)

        elif self.health > self.healthMeas * 0.2:
            self.colour = (255, 165, 0)

        else:
            self.colour = (255, 0, 0)

        pygame.draw.rect(self.window, (0, 0, 0), (25, 50, 200, 50), 0)
        pygame.draw.rect(self.window, self.colour, (25, 50, (self.health / self.healthMeas) * 200, 50), 0)

    def choose_action(self):
        self.action = randint(1, 100)

        if 1 <= self.action < 40:
            self.choseAction = 7

        elif 40 <= self.action < 70:
            self.magic = randint(1, 90)

            if self.magic <= 30:
                self.choseAction = 3

            elif self.magic <= 60:
                self.choseAction = 4

            else:
                self.choseAction = 5

        elif 70 <= self.action < 90:
            self.item = randint(1, 100)

            if self.item <= 50:
                self.choseAction = 1

            else:
                self.choseAction = 2

        else:
            self.choseAction = 6

    def reflector(self):
        pass

    def absorber(self):
        pass

    def magic_attack(self):
        pass

    def magic_defence(self):
        pass

    def magic_health(self):
        pass

    def taunt(self):
        pass

    def attack(self):
        pass


class Button:
    def __init__(self, colour, x, y, width, height, screen, text=""):
        self.colour = colour
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.window = screen
        self.text = text

    def draw(self, outline=False):
        if outline:
            pygame.draw.rect(self.window, outline, (self.x - 2, self.y - 2, self.width + 4, self.height + 4), 0)

        pygame.draw.rect(self.window, self.colour, (self.x, self.y, self.width, self.height), 0)

        if self.text != "":
            font = pygame.font.SysFont("comicsans", 30)
            text = font.render(self.text, True, (0, 0, 0))
            self.window.blit(text, (self.x + (self.width / 2 - text.get_width() / 2), self.y +
                                    (self.height / 2 - text.get_height() / 2)))

    def is_over(self, position):
        if self.x < position[0] < self.x + self.width:
            if self.y < position[1] < self.y + self.height:
                return True

        return False


def text_box(text):
    global passed

    pygame.draw.rect(window, (255, 255, 255), (0, 250, 500, 175), 0)
    pygame.draw.rect(window, (0, 0, 0), (2, 252, 496, 171), 0)

    if text == "r":
        text_on_screen = user.reflector(1, 1)

    elif text == "a":
        text_on_screen = user.absorber(1, 1)

    elif text == "ma":
        text_on_screen = user.magic_attack()

    elif text == "md":
        text_on_screen = user.magic_defence()

    elif text == "mh":
        text_on_screen = user.magic_health()

    elif text == "t":
        text_on_screen = user.taunt()

    else:
        text_on_screen = user.attack(1, 1)

    font = pygame.font.SysFont("didot", 25)
    text = font.render(text_on_screen, True, (255, 255, 255))
    window.blit(text, (5, 255))


def update_window(text="r"):
    global count
    global first
    global now

    itemButton.draw(outline=True)
    magicButton.draw(outline=True)
    tauntButton.draw(outline=True)
    attackButton.draw(outline=True)

    if wait:
        attackWaitButton.draw(outline=True)

        if first:
            first = False
            count = randint(10000, 20000)

        count -= 1

        if count <= 0:
            attackNowButton.draw(outline=True)

            if not now:
                now = time()

    elif done:
        text_box(text)

    elif useItemButton:
        itemReflectorButton.draw(outline=True)
        itemAbsorberButton.draw(outline=True)
        backButton.draw(outline=True)

    elif useMagicButton:
        magicAttackButton.draw(outline=True)
        magicDefenceButton.draw(outline=True)
        magicHealthButton.draw(outline=True)
        backButton.draw(outline=True)

    elif useTauntButton:
        tauntAllButton.draw(outline=True)
        backButton.draw(outline=True)

    elif useAttackButton:
        attack1Button.draw(outline=True)
        backButton.draw(outline=True)

    opp1.draw()
    opp1.health_bar()

    user.health_bar()

    pygame.display.update()


run = True
done = False
useItemButton = False
useMagicButton = False
useTauntButton = False
useAttackButton = False
wait = False
now = 0
passed = 0
first = False
count = 0
miss = False

attackButton = Button((0, 255, 0), 375, 425, 125, 75, window, text="Attack")
attack1Button = Button((0, 255, 0), 0, 350, 125, 75, window, text="Opp 1")
attack2Button = Button((0, 255, 0), 125, 350, 125, 75, window, text="Opp 2")
attack3Button = Button((0, 255, 0), 250, 350, 125, 75, window, text="Opp 3")
attackWaitButton = Button((0, 255, 0), 200, 275, 100, 75, window, text="Wait...")
attackNowButton = Button((255, 0, 0), 200, 275, 100, 75, window, text="NOW!!!")

magicButton = Button((0, 255, 0), 125, 425, 125, 75, window, text="Magic")
magicAttackButton = Button((0, 255, 0), 0, 350, 125, 75, window, text="Attack")
magicDefenceButton = Button((0, 255, 0), 125, 350, 125, 75, window, text="Defence")
magicHealthButton = Button((0, 255, 0), 250, 350, 125, 75, window, text="Health")

itemButton = Button((0, 255, 0), 0, 425, 125, 75, window, text="Items")
itemReflectorButton = Button((0, 255, 0), 125, 350, 125, 75, window, text="Reflector")
itemAbsorberButton = Button((0, 255, 0), 250, 350, 125, 75, window, text="Absorber")

tauntButton = Button((0, 255, 0), 250, 425, 125, 75, window, text="Taunt")
tauntAllButton = Button((0, 255, 0), 250, 350, 125, 75, window, text="All")

backButton = Button((0, 255, 0), 375, 350, 125, 75, window, text="Back")

user = User(1, 1, "McDaMastR", window)
opp1 = Enemy(window, 186, 122, 1)


def main():
    global run
    global done
    global useItemButton
    global useMagicButton
    global useTauntButton
    global useAttackButton
    global wait
    global now
    global passed
    global first
    global count
    global miss
    global attackButton
    global attack1Button
    global attack2Button
    global attack3Button
    global magicButton
    global magicAttackButton
    global magicDefenceButton
    global magicHealthButton
    global itemButton
    global itemReflectorButton
    global itemAbsorberButton
    global tauntButton
    global tauntAllButton
    global backButton

    while run:
        pygame.time.delay(50)

        window.fill((150, 0, 255))

        opp1.choose_action()

        for event in pygame.event.get():
            pos = pygame.mouse.get_pos()

            if event.type == pygame.QUIT:
                run = False
                pygame.quit()
                quit()

            if event.type == pygame.MOUSEBUTTONDOWN:
                if itemButton.is_over(pos):
                    useItemButton = True
                    useMagicButton = False
                    useTauntButton = False
                    useAttackButton = False

                elif magicButton.is_over(pos):
                    useItemButton = False
                    useMagicButton = True
                    useTauntButton = False
                    useAttackButton = False

                elif tauntButton.is_over(pos):
                    useItemButton = False
                    useMagicButton = False
                    useTauntButton = True
                    useAttackButton = False

                elif attackButton.is_over(pos):
                    useItemButton = False
                    useMagicButton = False
                    useTauntButton = False
                    useAttackButton = True

                if useItemButton:
                    if itemReflectorButton.is_over(pos):
                        done = True

                    elif itemAbsorberButton.is_over(pos):
                        done = True

                    elif backButton.is_over(pos):
                        useItemButton = False

                elif useMagicButton:
                    if magicAttackButton.is_over(pos):
                        done = True

                    elif magicDefenceButton.is_over(pos):
                        done = True

                    elif magicHealthButton.is_over(pos):
                        done = True

                    elif backButton.is_over(pos):
                        useMagicButton = False

                elif useTauntButton:
                    if tauntAllButton.is_over(pos):
                        done = True

                    elif backButton.is_over(pos):
                        useTauntButton = False

                elif useAttackButton:
                    if attack1Button.is_over(pos):
                        done = True
                        wait = True

                    elif backButton.is_over(pos):
                        useAttackButton = False

                elif wait:
                    if count:
                        if attackWaitButton.is_over(pos):
                            miss = True

                    else:
                        if attackNowButton.is_over(pos):
                            passed = now - time()
                            wait = False

            if event.type == pygame.MOUSEMOTION:
                if not done:
                    if attackButton.is_over(pos):
                        attackButton.colour = (255, 0, 0)

                    if magicButton.is_over(pos):
                        magicButton.colour = (255, 0, 0)

                    if itemButton.is_over(pos):
                        itemButton.colour = (255, 0, 0)

                    if tauntButton.is_over(pos):
                        tauntButton.colour = (255, 0, 0)

                    if not attackButton.is_over(pos):
                        attackButton.colour = (0, 255, 0)

                    if not magicButton.is_over(pos):
                        magicButton.colour = (0, 255, 0)

                    if not itemButton.is_over(pos):
                        itemButton.colour = (0, 255, 0)

                    if not tauntButton.is_over(pos):
                        tauntButton.colour = (0, 255, 0)

                    if useItemButton:
                        if itemReflectorButton.is_over(pos):
                            itemReflectorButton.colour = (255, 0, 0)

                        if itemAbsorberButton.is_over(pos):
                            itemAbsorberButton.colour = (255, 0, 0)

                        if backButton.is_over(pos):
                            backButton.colour = (255, 0, 0)

                        if not itemReflectorButton.is_over(pos):
                            itemReflectorButton.colour = (0, 255, 0)

                        if not itemAbsorberButton.is_over(pos):
                            itemAbsorberButton.colour = (0, 255, 0)

                        if not backButton.is_over(pos):
                            backButton.colour = (0, 255, 0)

                    elif useMagicButton:
                        if magicAttackButton.is_over(pos):
                            magicAttackButton.colour = (255, 0, 0)

                        if magicDefenceButton.is_over(pos):
                            magicDefenceButton.colour = (255, 0, 0)

                        if magicHealthButton.is_over(pos):
                            magicHealthButton.colour = (255, 0, 0)

                        if backButton.is_over(pos):
                            backButton.colour = (255, 0, 0)

                        if not magicAttackButton.is_over(pos):
                            magicAttackButton.colour = (0, 255, 0)

                        if not magicDefenceButton.is_over(pos):
                            magicDefenceButton.colour = (0, 255, 0)

                        if not magicHealthButton.is_over(pos):
                            magicHealthButton.colour = (0, 255, 0)

                        if not backButton.is_over(pos):
                            backButton.colour = (0, 255, 0)

                    elif useTauntButton:
                        if tauntAllButton.is_over(pos):
                            tauntAllButton.colour = (255, 0, 0)

                        if backButton.is_over(pos):
                            backButton.colour = (255, 0, 0)

                        if not tauntAllButton.is_over(pos):
                            tauntAllButton.colour = (0, 255, 0)

                        if not backButton.is_over(pos):
                            backButton.colour = (0, 255, 0)

                    elif useAttackButton:
                        if attack1Button.is_over(pos):
                            attack1Button.colour = (255, 0, 0)

                        if backButton.is_over(pos):
                            backButton.colour = (255, 0, 0)

                        if not attack1Button.is_over(pos):
                            attack1Button.colour = (0, 255, 0)

                        if not backButton.is_over(pos):
                            backButton.colour = (0, 255, 0)

        update_window()


if __name__ == "__main__":
    main()
