#include <numbers>
#include <cmath>
#include <array>
#include <ctime>

#include <doge/doge.hpp>

struct PongGame
{
    struct PaddleControl
    {
        doge::event::Key::Code up_key;
        doge::event::Key::Code down_key;
    };

    struct Board
    {
        doge::Vec2f size;
    };

    struct Ball
    {
        doge::Vec2f velocity;
    };

    struct PlayerScore
    {
        std::uint32_t score = 0;
        doge::Text* text;
    };

    doge::Engine& engine;

    Board board = Board({ 1280, 720 });

    inline static constexpr std::uint8_t player_count = 2;
    std::array<PlayerScore, player_count> player_scores = { 0 };

    doge::Vec2f paddle_size = { 30, 160 };
    float paddle_offset = 120.f;
    float paddle_speed = 1.f;

    doge::Color background_color = 0xFFFFFF88;
    float background_thickness = 10.f;

    const float default_ball_speed = .5f;
    const doge::Vec2f default_ball_size = { 30.f, 30.f };

    PongGame(doge::Engine& engine) : engine(engine)
    {
    }

    doge::Entity CreatePaddle(std::uint8_t player)
    {
        auto paddle_entity = engine.AddEntity();

        auto side = player ? -1 : 1;

        auto& position = paddle_entity.AddComponent<doge::Position>((board.size.x / 2.f - paddle_offset) * side, 0);

        paddle_entity.AddComponent(doge::RectangleShape
            {
                .size = paddle_size,
                .origin = paddle_size / 2.f,
            });

        auto up = player ? doge::event::Key::Code::W : doge::event::Key::Code::Up;
        auto down = player ? doge::event::Key::Code::S : doge::event::Key::Code::Down;

        paddle_entity.AddComponent<PaddleControl>(up, down);

        return paddle_entity;
    }

    doge::Entity SpawnBall()
    {
        auto ball_entity = engine.AddEntity();

        ball_entity.AddComponent<doge::Position>(0, std::rand() % static_cast<int>(board.size.y) - board.size.y / 2.f);

        auto& a = ball_entity.AddComponent<Ball>(
            doge::Vec2f(default_ball_speed, 0).Rotated(doge::cast::ToRadian(std::rand() % 360))
        );

        ball_entity.AddComponent(doge::RectangleShape
            {
                .size = default_ball_size,
                .origin = default_ball_size / 2.f,
            });

        return ball_entity;
    }

    void Start(doge::Engine& engine)
    {
        // camera
        auto cam_entity = engine.AddCamera();
        cam_entity.AddComponent(doge::Layer::Create(-1, 0));

        // background
        {
            auto background = engine.AddEntity();

            auto board_border = engine.AddEntity();
            board_border.SetParent(background);
            board_border.AddComponent(doge::Layer::Create(-1));
            board_border.AddComponent(doge::RectangleShape
                {
                    .size = board.size,
                    .origin = board.size / 2.f,
                    .color = doge::Color::Black,
                    .outline_color = background_color,
                    .outline_thickness = background_thickness,
                });

            auto mid_line = engine.AddEntity();
            mid_line.SetParent(background);
            mid_line.AddComponent(doge::Layer::Create(-1));
            mid_line.AddComponent(doge::RectangleShape
                {
                    .size = { background_thickness, board.size.y },
                    .origin = { background_thickness / 2.f, board.size.y / 2.f },
                    .color = background_color,
                });

            auto player0_score = engine.AddEntity();
            player0_score.SetParent(background);
            player0_score.AddComponent(doge::Position(-20, -board.size.y / 2.f + 20));
            auto& text0 = player0_score.AddComponent(doge::Text
                {
                    .string = U"0",
                    .font_size = 50,
                    .align = doge::Text::Align::Right,
                    .character_appearances = 
                    {
                        { 0, doge::Text::Appearance
                            {
                                .fill_color = background_color,
                            }
                        }
                    },
                });
            player_scores.at(0) = PlayerScore(0, &text0);

            auto player1_score = engine.AddEntity();
            player1_score.SetParent(background);
            player1_score.AddComponent(doge::Position(20, -board.size.y / 2.f + 20));
            auto& text1 = player1_score.AddComponent(doge::Text
                {
                    .string = U"0",
                    .font_size = 50,
                    .align = doge::Text::Align::Left,
                    .character_appearances = 
                    {
                        { 0, doge::Text::Appearance
                            {
                                .fill_color = background_color,
                            }
                        }
                    },
                });
            player_scores.at(1) = PlayerScore(0, &text1);
        }

        // paddles
        CreatePaddle(0);
        CreatePaddle(1);

        // ball
        SpawnBall();
    }

    void Update(doge::Engine& engine, doge::DeltaTime dt)
    {
        // paddle control
        for (auto [ctrl, pos] : engine.Select<PaddleControl, doge::Position>().Components())
        {
            if (doge::io::Keyboard::IsKeyDown(ctrl.up_key))
            {
                pos.position.y -= paddle_speed * dt;

                if (pos.position.y < -board.size.y / 2.f + paddle_size.y / 2.f)
                {
                    pos.position.y = -board.size.y / 2.f + paddle_size.y / 2.f;
                }
            }

            if (doge::io::Keyboard::IsKeyDown(ctrl.down_key))
            {
                pos.position.y += paddle_speed * dt;

                if (pos.position.y > board.size.y / 2.f - paddle_size.y / 2.f)
                {
                    pos.position.y = board.size.y / 2.f - paddle_size.y / 2.f;
                }
            }
        }
    }

    void FixedUpdate(doge::Engine& engine, doge::DeltaTime dt)
    {
        // ball movement
        for (auto [ball, pos, rect] : engine.Select<Ball, doge::Position, doge::RectangleShape>().Components())
        {
            pos.position += ball.velocity * dt;
            ball.velocity += ball.velocity.Normalized() * .00005f * dt;

            // hit paddle

            for (auto [ctrl, paddle_pos, paddle_rect] : engine.Select<PaddleControl, doge::Position, doge::RectangleShape>().Components())
            {
                auto relative_pos = paddle_pos.position - pos.position;
                if (
                    (ball.velocity.x > 0) == (relative_pos.x > 0) &&
                    doge::global::GetAABB(rect).Overlap(doge::global::GetAABB(paddle_rect))
                    )
                {
                    pos.position.x = paddle_pos.position.x + (paddle_rect.size.x + rect.size.x) / 2.f * (relative_pos.x > 0 ? -1 : 1);
                    ball.velocity = (pos.position - paddle_pos.position).Normalized() * ball.velocity.Magnitude();
                }
            }

            // hit wall

            if (ball.velocity.y > 0 && pos.position.y >= (board.size.y - rect.size.y) / 2.f)
            {
                ball.velocity.y = -ball.velocity.y;
            }

            if (ball.velocity.y < 0 && pos.position.y <= (-board.size.y + rect.size.y) / 2.f)
            {
                ball.velocity.y = -ball.velocity.y;
            }

            // travel direction constraint

            if (ball.velocity.x == 0 || std::abs(ball.velocity.y) / std::abs(ball.velocity.x) > 2)
            {
                if (ball.velocity.x == 0)
                    ball.velocity.x = .001f;

                ball.velocity = doge::Vec2f(3, 1).Normalized() * (ball.velocity / ball.velocity.Abs()) * ball.velocity.Magnitude();
            }

            // goal check

            int goal = -1;
            if (pos.position.x > (board.size.x - rect.size.x) / 2.f)
            {
                goal = 0;
            }
            else if (pos.position.x < (-board.size.x + rect.size.x) / 2.f)
            {
                goal = 1;
            }

            if (goal != -1)
            {
                ++player_scores.at(goal).score;

                player_scores.at(goal).text->string.clear();
                for (auto c : std::to_string(player_scores.at(goal).score))
                {
                    player_scores.at(goal).text->string.push_back(c);
                }

                engine.DestroyEntity(ball.entity_id);
                SpawnBall();
            }
        }
    }
};

int main()
{
    srand(time(NULL));
    std::rand();
    std::rand();

    doge::Engine engine;
    engine.window.settings.size = doge::Vec2u(1280, 720);
    engine.window.settings.style = doge::Window::Settings::Style::Titlebar | doge::Window::Settings::Style::Close;
    engine.window.settings.fps = 60;
    engine.window.settings.title = "DOGE!";
    engine.scenes.fixed_time_step = 4;

    PongGame game(engine);
    doge::GameLoopFunctions glf
    {
        .start = std::bind(&PongGame::Start, &game, std::placeholders::_1),
        .fixed_update = std::bind(&PongGame::FixedUpdate, &game, std::placeholders::_1, std::placeholders::_2),
        .update = std::bind(&PongGame::Update, &game, std::placeholders::_1, std::placeholders::_2),
    };

    engine.AddScene("MyScene", glf);

    engine.StartScene("MyScene");

    return 0;
}
