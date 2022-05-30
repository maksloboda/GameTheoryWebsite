export const messages = {
    en: {
      message: {
        SekiSettings: {
            PassOptions: 'Pass options',
            RPass: 'R can pass',
            CPass: 'C can pass',
            GameType: 'Game type',
            FieldWidth: 'Field width',
            FieldHeight: 'Field height',
        },
        SingleSuitSettings: {
            Fool: 'Fool',
            DFool: 'D-Fool',
            FirstPlayer: 'First player',
            SecondPlayer: 'Second player',
            NthCard: '-th card',
            CardNumber: 'Number of cards',
        },
        SekiInterface: {
            NullGameStatus: 'Current game status is null',
            Pass: 'Pass',
        },
        SingleSuitInterface: {
            GameType: 'SingleSuit',
            NullGameState: 'SingleSuit state is null',
        },
        GameSelector: {
            CreateGame: 'Create a game',
            ConnectToGame: 'Join the game',
        },
        JoinGame: {
            LobbiesFound: 'Number of found lobbies:',
        },
        PlayGame: {
            WaitPlayers: 'Awaiting Players...',
            YourTurn: 'is current player - Your turn',
            OpponentsTurn: 'is current player - Opponent`s turn',
            Draw: 'Draw',
            IsWinner: 'is winner',
            CopyLink: 'Copy link',
            JoinAs: 'Join As',
            FirstMove: 'first move',
            CanPass: 'can pass',
            Start: 'Start:',
            Loading: 'Loading...',
            MakeMove: 'Make move',
            LeaveGame: 'Leave game',
            AgainstComputer: 'Play against computer',
            AgainstHuman: 'Play against human',
            Spectator: 'Spectator mode',
        },

        SekiInfo: {
          header: 'Rules of game Seki',
          part1: 'You are given non-negative matrix A of size m x n. Two\
          players(R and C, for example) are making moves alternately. By one move\
          a player can reduce by 1 any positive entry of A. R wants to create a zero \
          row, C wants to create a zero column. If there appear a zero row and zero column after some move\
          simultaneously there are two options.',
          option1: 'Game Seki: the player who made the last move wins (this version is closely related to the so-called Seki (Shared Life) positions of Go)',
          option2: 'Game D-seki results in a draw.',
          part2: 'We assume that the initial matrix does not contain zero-rows or zero-columns. If it does, the game is over before it begins.\
          For each game, Seki and D-Seki, there are also the following four options:',
          mode1: '(0) Players cannot pass',
          mode2: '(1R) can pass but C cannot',
          mode3: '(1C) can pass but R cannot',
          mode4: '(2) Both players can pass',
          endInfo: 'If both players pass successively, the game is over resulting in a draw'
        },
        SingleSuitInfo: {
          header: 'Info about SingleSuit game pattern',
          SuitWhistette: {
            header: 'SingleSuit-Whistette',
            info: 'Two players(let them be called A and B) are given 2k cards, k cards for \
            every player. It is easy to see that there are C(2k, k) number of determined start positions.\
            It is known who is on lead(let us say A). The player chooses one of his cards and puts it on\
            the table, the number up, so B clearly can see the card. After this B chooses one of his cards\
            The player whose card is larger wins the trick. Furthermore, this player is to lead in the next round.\
            In k rounds the game is over. Each player wants to maximize the number of tricks (s)he won, that is, kA\
            and kB, respectively. This is a constant sum game: kA + kB = k',
          },
          WeightedWhistette: {
            header: 'Weighted SingleSuit-Whistette',
            info: 'Given k integer numbers (weights) w1, . . . , wk taking values between −99 and 99. (Some of these values may be equal.)\
            Now the player who won the trick i gets wi points for it. (So (s)he loses some points when wi is negative.) Each player wants to maximize the number of points for the tricks (s)he won.\
            If each weight is equal to 1, we obtained the standard Whistette; if it is (-1) the game is called mis`ere Whistette.',
          },
          SuitFool: {
            header: 'Single-Suit Fool and D-Fool',
            part1: 'In this game two players somehow get k cards, its number may be different\
            for every player. Then it is known who starts the game, let us assume that B plays first.\
            He puts chosen card to the table, let it be card i. Then the player A has a choice:',
            option1: 'Take the card himself',
            option2: 'Play a card nominalo j, where j > i',
            part2: 'In second variant, two cards go out of the game, and can not be played anymore, A starts next move.\
            The player wins when his opponent is out of cards. His final score is number of opponent`s cards. If two players\
            are run out of cards simultaneously, then in basic version wins the player who\
            won the last play with score 1/2. And when we are talking about D-Fool, two players finish with score 0.',
          },
          WeightedSuitFool: {
            header: 'Weighted Single-Suit Fool and D-Fool',
            info: 'Players are given k numbers in interval from -99 to 99, where every number means a weight of i_th card and \
            finally the score of a player is a weighted sum of opponent`s cards. Let us assume that in a case of negative sum\
            player who is run out of cards loses. In addition, if weighted sum of opponent`s cards is equal to 0, then in a\
            basic version he gets score 1/2, in D-version he gets 0.',
          }
        }
      }
    },
    ru: {
      message: {
        SekiSettings: {
            PassOptions: 'Возможность пропуска хода',
            RPass: 'R может пропускать ход',
            CPass: 'C может пропускать ход',
            GameType: 'Тип игры',
            FieldWidth: 'Ширина поля',
            FieldHeight: 'Высота поля',
        },
        SingleSuitSettings: {
            Fool: 'Дурак',
            DFool: 'D-Дурак',
            FirstPlayer: 'Первый игрок',
            SecondPlayer: 'Второй игрок',
            NthCard: '-ая карта',
            CardNumber: 'Число карт в раздаче',
        },
        SekiInterface: {
            NullGameStatus: 'Текущее состояние игры неизвестно',
            Pass: 'Пропуск хода',
        },
        SingleSuitInterface: {
            GameType: 'Одномастка',
            NullGameState: 'Состояние игры неизвестно',
        },
        GameSelector: {
            CreateGame: 'Создать игру',
            ConnectToGame: 'Подключиться к лобби',
        },
        JoinGame: {
            LobbiesFound: 'Найдено лобби:',
        },
        PlayGame: {
            WaitPlayers: 'Ожидание игроков...',
            YourTurn: 'сейчас ваш ход',
            OpponentsTurn: 'сейчас ход вашего противника',
            Draw: 'Ничья',
            IsWinner: 'выиграл',
            CopyLink: 'Скопировать ссылку',
            JoinAs: 'Присоединиться как',
            FirstMove: 'Ходит первый',
            CanPass: 'может пропустить ход',
            Start: 'Запуск:',
            Loading: 'Загрузка...',
            MakeMove: 'Сделать ход',
            LeaveGame: 'Покинуть игру',
            AgainstComputer: 'Игра против бота',
            AgainstHuman: 'Против реального человека',
            Spectator: 'Наблюдатель',
        },
        SekiInfo: {
          header: 'Правила семейства игр Секи',
          part1: 'Задана матрица A размера m x n, состоящая из неотрицательных чисел.\
          Два игрока(обозначим их для удобства R и C), по очереди делают ходы.\
          За каждый ход один из игроков может уменьшить любое положительное\
          число матрицы на единицу.\
          Игрок R стремится занулить строку, игрок C - столбец. Если после\
          какого-то хода одна из строк и один из столбцов были занулены\
          одновременно, то существуют два варианта развития событий.',
          option1: 'Игра Seki: сделавший ход последним - выигрывает.',
          option2: 'Игра D-Seki: вышеуказанная ситуация является ничьей.',
          part2: 'При всем вышенаписанном, мы учитываем, что входная матрица\
          не имеет нулевых строк или столбцов(иначе игра закончена до её начала).\
    \
          Для каждой игры есть 4 различных вариации:',
          mode1: '(0): Игроки не могут пропускать ходы',
          mode2: '(1R): R может пасовать, C - нет',
          mode3: '(1C): C может пасовать, R - нет',
          mode4: '(2): оба игрока имеют право пасовать.',
          endInfo: 'Если в какой-то из циклов ходов оба игрока решили пропустить ход - \
          игра заканчивается ничьей.'
        },
        SingleSuitInfo: {
          header: 'Описание правил игры в семейство игр Одномастка',
          SuitWhistette: {
            header: 'Одномастка-вист',
            info: 'Всего есть 2k карт и 2 игрока A и B, каждый из которых получает k карт.\
            Соответственно, всего существует число сочетаний из 2k по k различных\
            комбинаций определяющих игру. Изначально определено кто начинает, не\
            умаляя общности будем считать, что A. Игрок берет одну из своих k карт\
            и кладет на стол, B видит карту. После того, как B выбрал какую карту \
            положить, игрок, который положил большую карту выигрывает ход. Далее,\
            победивший игрок ходит первым в следующем раунде. Через k раундов игрок,\
            выигравший большее число ходов - выигрывает. Каждый игрок стремится \
            максимизировать количество набранных очков k1 и k2 соответственно. Легко\
            заметить, что это игра с константной суммой, k1 + k2 = k.',
          },
          WeightedWhistette: {
            header: 'Взвешенная версия Одномастки-вист',
            info: 'Задаются k чисел в интервале от -99 до 99, где каждое число обозначает\
            количество очков, которые игроки получают, за победу на i-м ходу. Каждый\
            игрок хочет максимизировать число своих очков.\
            Таким образом в часте случаев мы вырождаемся либо в стандартную версию, либо в мизерную(все веса равны -1).',
          },
          SuitFool: {
            header: 'Одномастка-дурак и D-дурак',
            part1: 'В этой игре 2 игрока каким-либо способом получают k карт, их количество может быть различно\
            у обоих игроков. Далее так же известно, кто начинает заранее, не умаляя общности считаем, что\
            начинает B. Он кладет в "открытую" любую свою карту на стол(пусть ее номинал i). Далее у A есть выбор:',
            option1: 'Взять эту карту себе',
            option2: 'Сыграть карту j, такую, что j > i',
            part2: 'Во втором случае, обе карты выбывают, и больше не используются в игре, а следующий ход начинает A.\
            Выигрывает игрок, у которого первым кончились карты. Его счет - число карт у оппонента. Если у обоих игроков\
            кончились карты одновременно, в таком случае, если мы играем в обычную версию, то побеждает игрок, выигравший\
            последний ход со счетом 1/2. В ином случае, при игре в D-дурак, оба игрока заканчивают со счетом 0.',
          },
          WeightedSuitFool: {
            header: 'Взвешенные одномастка-дурак и D-дурак',
            info: 'Задаются k чисел в интервале от -99 до 99, где каждое число обозначает вес i-й карточки и \
            в итоге счет избавившегося от карт игрока - это сумма весов карточек оппонента. Заметим, что если сумма отрицательная,\
            то избавившийся от карт игрок - проигрывает. Так же, если сумма весов для избавившегося - 0, то в обычной\
            версии он получает 1/2, в D-дураке - 0.',
          }
        }
      }
    }
  }
  