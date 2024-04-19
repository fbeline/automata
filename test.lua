function ab()
  press_key(string.byte('A'))
  wait(50)
  release_key(string.byte('A'))

  press_key(66)
  wait(50)
  release_key(66)
end

function cd()
  press_key(string.byte('C'))
  wait(50)
  release_key(string.byte('C'))

  press_key(string.byte('D'))
  wait(50)
  release_key(string.byte('D'))
end

actions = {
  { keycode = 113, action = "ab" },
  { keycode = 114, action = "cd" }
}
