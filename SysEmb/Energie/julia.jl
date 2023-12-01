f(z, c) = z*z + c
function is_stable(iter, z, c)
    for _ in 1:iter
        if abs(z) > 2
            return false
        end
        z = f(z, c)
    end
    true
end

function julia(precision, X, Y, c, f)
    Points = Tuple{Float64, Float64}[]
    for x in X
        for y in Y
            z = f(x, y)
            if is_stable(precision, z, c)
                push!(Points, (x, y))
            end
        end
    end
    scatter(Points, markersize=4)
end

mandel(80, -2:0.0025:2, -2:0.0025:2, Complex) |> display
sleep(10)
julia(80, -2:0.0005:2, -2:0.0005:2, -0.8im, Complex) |> display
