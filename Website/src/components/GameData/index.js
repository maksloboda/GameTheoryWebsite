import Seki from "./Seki"
import SingleSuit from "./SingleSuit"
import Whistette from "./SingleSuit/Whistette"
import Fool from "./SingleSuit/Fool"

export default {
    games: [
        Seki,
        Whistette,
        Fool,
    ],

    game_cards: [
        Seki,
        SingleSuit
    ]
}